o/imc.o: imc.c h/sha256.h globals.h typedefs.h ack.h h/config.h utils.h \
  h/imc.h h/imccfg.h lists.h strfuns.h mquest.h h/act_wiz.h h/comm.h
o/sha256.o: sha256.c h/sha256.h
o/act_clan.o: act_clan.c globals.h typedefs.h ack.h h/config.h utils.h \
  h/imc.h h/imccfg.h lists.h strfuns.h mquest.h h/act_clan.h h/act_comm.h \
  h/act_info.h h/act_wiz.h h/comm.h
o/act_comm.o: act_comm.c globals.h typedefs.h ack.h h/config.h utils.h \
  h/imc.h h/imccfg.h lists.h strfuns.h mquest.h h/act_comm.h h/act_info.h \
  h/build.h h/comm.h money.h
o/act_info.o: act_info.c globals.h typedefs.h ack.h h/config.h utils.h \
  h/imc.h h/imccfg.h lists.h strfuns.h mquest.h h/act_comm.h h/act_info.h \
  h/act_obj.h h/act_wiz.h h/board.h h/buildtab.h h/comm.h h/cursor.h \
  magic.h money.h
o/act_mob.o: act_mob.c globals.h typedefs.h ack.h h/config.h utils.h \
  h/imc.h h/imccfg.h lists.h strfuns.h mquest.h h/act_comm.h h/act_mob.h \
  h/act_move.h h/act_obj.h h/act_wiz.h
o/act_move.o: act_move.c globals.h typedefs.h ack.h h/config.h utils.h \
  h/imc.h h/imccfg.h lists.h strfuns.h mquest.h h/act_comm.h h/act_info.h \
  h/act_mob.h h/act_move.h h/build.h h/comm.h
o/act_obj.o: act_obj.c globals.h typedefs.h ack.h h/config.h utils.h \
  h/imc.h h/imccfg.h lists.h strfuns.h mquest.h h/act_comm.h h/act_wiz.h \
  h/comm.h money.h magic.h
o/act_wiz.o: act_wiz.c globals.h typedefs.h ack.h h/config.h utils.h \
  h/imc.h h/imccfg.h lists.h strfuns.h mquest.h h/act_comm.h h/act_info.h \
  h/act_mob.h h/act_obj.h h/act_wiz.h h/areasave.h h/build.h h/buildtab.h \
  h/comm.h email.h
o/areachk.o: areachk.c globals.h typedefs.h ack.h h/config.h utils.h \
  h/imc.h h/imccfg.h lists.h strfuns.h mquest.h hash.h h/areasave.h \
  h/comm.h
o/areasave.o: areasave.c globals.h typedefs.h ack.h h/config.h utils.h \
  h/imc.h h/imccfg.h lists.h strfuns.h mquest.h h/act_wiz.h h/areasave.h \
  h/build.h h/comm.h
o/board.o: board.c globals.h typedefs.h ack.h h/config.h utils.h h/imc.h \
  h/imccfg.h lists.h strfuns.h mquest.h h/board.h h/build.h h/comm.h
o/build.o: build.c globals.h typedefs.h ack.h h/config.h utils.h h/imc.h \
  h/imccfg.h lists.h strfuns.h mquest.h h/act_info.h h/act_move.h \
  h/act_wiz.h h/areachk.h h/areasave.h h/build.h h/buildare.h \
  h/buildtab.h h/comm.h money.h
o/buildare.o: buildare.c globals.h typedefs.h ack.h h/config.h utils.h \
  h/imc.h h/imccfg.h lists.h strfuns.h mquest.h h/areasave.h h/build.h \
  h/buildare.h h/comm.h money.h
o/buildtab.o: buildtab.c globals.h typedefs.h ack.h h/config.h utils.h \
  h/imc.h h/imccfg.h lists.h strfuns.h mquest.h
o/clutch.o: clutch.c globals.h typedefs.h ack.h h/config.h utils.h h/imc.h \
  h/imccfg.h lists.h strfuns.h mquest.h h/act_comm.h h/act_info.h \
  h/clutch.h h/comm.h
o/comm.o: comm.c globals.h typedefs.h ack.h h/config.h utils.h h/imc.h \
  h/imccfg.h lists.h strfuns.h mquest.h h/act_comm.h h/act_info.h \
  h/act_obj.h h/act_wiz.h h/comm.h h/cursor.h
o/const.o: const.c globals.h typedefs.h ack.h h/config.h utils.h h/imc.h \
  h/imccfg.h lists.h strfuns.h mquest.h magic.h money.h
o/ctor.o: ctor.c globals.h typedefs.h ack.h h/config.h utils.h h/imc.h \
  h/imccfg.h lists.h strfuns.h mquest.h money.h
o/db.o: db.c globals.h typedefs.h ack.h h/config.h utils.h h/imc.h \
  h/imccfg.h lists.h strfuns.h mquest.h hash.h ssm.h money.h h/act_info.h \
  h/act_mob.h h/act_obj.h h/act_wiz.h h/comm.h
o/dtor.o: dtor.c globals.h typedefs.h ack.h h/config.h utils.h h/imc.h \
  h/imccfg.h lists.h strfuns.h mquest.h h/act_wiz.h h/comm.h
o/email.o: email.c globals.h typedefs.h ack.h h/config.h utils.h h/imc.h \
  h/imccfg.h lists.h strfuns.h mquest.h h/act_comm.h h/act_wiz.h h/comm.h \
  email.h
o/enchant.o: enchant.c globals.h typedefs.h ack.h h/config.h utils.h \
  h/imc.h h/imccfg.h lists.h strfuns.h mquest.h h/act_comm.h h/buildtab.h \
  h/comm.h
o/fight.o: fight.c globals.h typedefs.h ack.h h/config.h utils.h h/imc.h \
  h/imccfg.h lists.h strfuns.h mquest.h h/act_comm.h h/act_info.h \
  h/act_mob.h h/act_move.h h/act_obj.h h/act_wiz.h h/buildtab.h h/comm.h \
  money.h
o/handler.o: handler.c globals.h typedefs.h ack.h h/config.h utils.h \
  h/imc.h h/imccfg.h lists.h strfuns.h mquest.h h/act_comm.h h/act_info.h \
  h/act_mob.h h/act_move.h h/act_obj.h h/act_wiz.h h/comm.h money.h
o/hash.o: hash.c globals.h typedefs.h ack.h h/config.h utils.h h/imc.h \
  h/imccfg.h lists.h strfuns.h mquest.h hash.h
o/hunt.o: hunt.c globals.h typedefs.h ack.h h/config.h utils.h h/imc.h \
  h/imccfg.h lists.h strfuns.h mquest.h h/act_comm.h h/act_move.h \
  h/act_wiz.h h/comm.h
o/interp.o: interp.c globals.h typedefs.h ack.h h/config.h utils.h h/imc.h \
  h/imccfg.h lists.h strfuns.h mquest.h h/act_clan.h h/act_comm.h \
  h/act_info.h h/act_move.h h/act_obj.h h/act_wiz.h h/board.h h/build.h \
  h/buildare.h h/clutch.h h/comm.h magic.h money.h
o/lists.o: lists.c globals.h typedefs.h ack.h h/config.h utils.h h/imc.h \
  h/imccfg.h lists.h strfuns.h mquest.h
o/macros.o: macros.c globals.h typedefs.h ack.h h/config.h utils.h h/imc.h \
  h/imccfg.h lists.h strfuns.h mquest.h h/act_wiz.h h/comm.h
o/magic.o: magic.c globals.h typedefs.h ack.h h/config.h utils.h h/imc.h \
  h/imccfg.h lists.h strfuns.h mquest.h h/act_comm.h h/act_info.h \
  h/act_move.h h/act_obj.h h/act_wiz.h h/buildtab.h h/comm.h magic.h \
  money.h
o/mapper.o: mapper.c mapper.h globals.h typedefs.h ack.h h/config.h utils.h \
  h/imc.h h/imccfg.h lists.h strfuns.h mquest.h h/act_comm.h h/comm.h \
  h/cursor.h
o/mob_commands.o: mob_commands.c globals.h typedefs.h ack.h h/config.h \
  utils.h h/imc.h h/imccfg.h lists.h strfuns.h mquest.h h/act_wiz.h \
  h/comm.h
o/mob_prog.o: mob_prog.c globals.h typedefs.h ack.h h/config.h utils.h \
  h/imc.h h/imccfg.h lists.h strfuns.h mquest.h h/comm.h
o/money.o: money.c globals.h typedefs.h ack.h h/config.h utils.h h/imc.h \
  h/imccfg.h lists.h strfuns.h mquest.h h/act_comm.h h/act_wiz.h h/comm.h \
  money.h
o/mount.o: mount.c globals.h typedefs.h ack.h h/config.h utils.h h/imc.h \
  h/imccfg.h lists.h strfuns.h mquest.h h/act_comm.h h/comm.h
o/mquest.o: mquest.c globals.h typedefs.h ack.h h/config.h utils.h h/imc.h \
  h/imccfg.h lists.h strfuns.h mquest.h h/act_comm.h h/comm.h money.h
o/obj_fun.o: obj_fun.c globals.h typedefs.h ack.h h/config.h utils.h \
  h/imc.h h/imccfg.h lists.h strfuns.h mquest.h h/comm.h
o/pdelete.o: pdelete.c globals.h typedefs.h ack.h h/config.h utils.h \
  h/imc.h h/imccfg.h lists.h strfuns.h mquest.h h/comm.h
o/quest.o: quest.c globals.h typedefs.h ack.h h/config.h utils.h h/imc.h \
  h/imccfg.h lists.h strfuns.h mquest.h h/act_comm.h h/act_wiz.h h/comm.h \
  money.h
o/rulers.o: rulers.c globals.h typedefs.h ack.h h/config.h utils.h h/imc.h \
  h/imccfg.h lists.h strfuns.h mquest.h h/act_wiz.h h/comm.h
o/scheck.o: scheck.c globals.h typedefs.h ack.h h/config.h utils.h h/imc.h \
  h/imccfg.h lists.h strfuns.h mquest.h h/comm.h ssm.h
o/save.o: save.c globals.h typedefs.h ack.h h/config.h utils.h h/imc.h \
  h/imccfg.h lists.h strfuns.h mquest.h hash.h h/act_obj.h h/act_wiz.h \
  h/comm.h money.h
o/social-edit.o: social-edit.c globals.h typedefs.h ack.h h/config.h \
  utils.h h/imc.h h/imccfg.h lists.h strfuns.h mquest.h h/act_info.h \
  h/comm.h
o/special.o: special.c globals.h typedefs.h ack.h h/config.h utils.h \
  h/imc.h h/imccfg.h lists.h strfuns.h mquest.h h/act_comm.h h/act_info.h \
  h/act_move.h h/act_obj.h h/act_wiz.h h/comm.h magic.h money.h
o/spell_dam.o: spell_dam.c globals.h typedefs.h ack.h h/config.h utils.h \
  h/imc.h h/imccfg.h lists.h strfuns.h mquest.h h/act_comm.h h/act_info.h \
  h/act_move.h h/act_obj.h h/act_wiz.h h/comm.h magic.h money.h
o/spendqp.o: spendqp.c globals.h typedefs.h ack.h h/config.h utils.h \
  h/imc.h h/imccfg.h lists.h strfuns.h mquest.h h/act_comm.h h/act_info.h \
  h/act_wiz.h h/build.h h/comm.h
o/ssm.o: ssm.c globals.h typedefs.h ack.h h/config.h utils.h h/imc.h \
  h/imccfg.h lists.h strfuns.h mquest.h ssm.h
o/strfuns.o: strfuns.c globals.h typedefs.h ack.h h/config.h utils.h \
  h/imc.h h/imccfg.h lists.h strfuns.h mquest.h h/act_wiz.h
o/sysdata.o: sysdata.c globals.h typedefs.h ack.h h/config.h utils.h \
  h/imc.h h/imccfg.h lists.h strfuns.h mquest.h h/act_info.h h/act_wiz.h \
  h/comm.h
o/trigger.o: trigger.c globals.h typedefs.h ack.h h/config.h utils.h \
  h/imc.h h/imccfg.h lists.h strfuns.h mquest.h h/act_info.h h/comm.h
o/update.o: update.c globals.h typedefs.h ack.h h/config.h utils.h h/imc.h \
  h/imccfg.h lists.h strfuns.h mquest.h h/act_comm.h h/act_move.h \
  h/act_obj.h h/act_wiz.h h/areasave.h h/comm.h money.h
o/vampire.o: vampire.c globals.h typedefs.h ack.h h/config.h utils.h \
  h/imc.h h/imccfg.h lists.h strfuns.h mquest.h h/act_info.h h/act_move.h \
  h/comm.h
o/werewolf.o: werewolf.c globals.h typedefs.h ack.h h/config.h utils.h \
  h/imc.h h/imccfg.h lists.h strfuns.h mquest.h h/act_comm.h h/act_move.h \
  h/comm.h
o/wizutil.o: wizutil.c globals.h typedefs.h ack.h h/config.h utils.h \
  h/imc.h h/imccfg.h lists.h strfuns.h mquest.h h/comm.h
o/write.o: write.c globals.h typedefs.h ack.h h/config.h utils.h h/imc.h \
  h/imccfg.h lists.h strfuns.h mquest.h h/comm.h
