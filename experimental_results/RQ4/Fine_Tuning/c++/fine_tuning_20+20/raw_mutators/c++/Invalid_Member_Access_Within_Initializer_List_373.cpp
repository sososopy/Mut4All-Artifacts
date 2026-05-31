//header file
#pragma once
#include "Mutator_base.h"

/**
 * Invalid_Member_Access_Within_Initializer_List_373
 */ 
class MutatorFrontendAction_373 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(373)

private:
    class MutatorASTConsumer_373 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_373(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        std::vector<const clang::RecordDecl *> cur_structs;
    };
};

//source file
#include "../include/Invalid_Member_Access_Within_Initializer_List_373.h"

// ========================================================================================================
#define MUT373_OUTPUT 1

void MutatorFrontendAction_373::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ST = Result.Nodes.getNodeAs<clang::RecordDecl>("Structs")) {
        if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                       ST->getLocation()))
          return;
        if (!ST->isStruct())
          return;
        cur_structs.push_back(ST);
      } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("Decls")) {
        if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                       DL->getLocation()))
          return;
        if (!DL->getType()->isStructureType())
          return;
        auto init = DL->getInit();
        if (!init)
          return;
        if (init->getStmtClass() != Stmt::StmtClass::InitListExprClass)
          return;
        auto inits = DL->getInit()->children();
        for (auto init : inits) {
          if (init->getStmtClass() != Stmt::StmtClass::DesignatedInitExprClass)
            continue;
          auto DLinit = dyn_cast<DesignatedInitExpr>(init);
          auto DLinits = DLinit->children();
          for (auto DLinit : DLinits) {
            if (DLinit->getStmtClass() != Stmt::StmtClass::MemberExprClass)
              continue;
            auto MB = dyn_cast<MemberExpr>(DLinit);
            auto base = MB->getBase();
            if (base->getStmtClass() != Stmt::StmtClass::ImplicitCastExprClass)
              continue;
            auto cast = dyn_cast<ImplicitCastExpr>(base);
            auto subexpr = cast->getSubExpr();
            if (subexpr->getStmtClass() != Stmt::StmtClass::DeclRefExprClass)
              continue;
            auto DLref = dyn_cast<DeclRefExpr>(subexpr);
            auto DLname = DLref->getNameInfo().getAsString();
            llvm::outs() << DLname << '\n';
            auto membername = MB->getMemberNameInfo().getAsString();
            llvm::outs() << membername << '\n';
            if (cur_structs.empty())
              return;
            int index = getrandom::getRandomIndex(cur_structs.size() - 1);
            auto target = cur_structs[index];
            auto targetname = target->getNameAsString();
            llvm::outs() << targetname << '\n';
            auto fields = target->fields();
            std::vector<string> fieldnames;
            for (auto field : fields) {
              fieldnames.push_back(field->getNameAsString());
            }
            auto DLtype = DL->getType().getAsString();
            DLtype = DLtype.substr(0, DLtype.find('*'));
            if (DLtype == targetname) {
              if (fieldnames.empty()) {
                Rewrite.ReplaceText(MB->getMemberLoc(), membername.size(),
                                    "mut373");
              } else {
                int choice = getrandom::getRandomIndex(fieldnames.size() - 1);
                Rewrite.ReplaceText(MB->getMemberLoc(), membername.size(),
                                    fieldnames[choice]);
              }
            } else {
              Rewrite.ReplaceText(DLref->getBeginLoc(), DLname.size(),
                                  targetname);
              if (fieldnames.empty()) {
                Rewrite.ReplaceText(MB->getMemberLoc(), membername.size(),
                                    "mut373");
              } else {
                int choice = getrandom::getRandomIndex(fieldnames.size() - 1);
                Rewrite.ReplaceText(MB->getMemberLoc(), membername.size(),
                                    fieldnames[choice]);
              }
            }
          }
        }
      }
}
  
void MutatorFrontendAction_373::MutatorASTConsumer_373::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = recordDecl().bind("Structs");
    auto decl_matcher = varDecl().bind("Decls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(decl_matcher, &callback);
    matchFinder.matchAST(Context);
}