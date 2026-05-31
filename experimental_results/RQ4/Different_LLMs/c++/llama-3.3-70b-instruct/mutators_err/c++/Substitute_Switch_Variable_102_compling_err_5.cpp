//header file
#pragma once
#include "Mutator_base.h"

/**
 * Substitute_Switch_Variable_102
 */ 
class MutatorFrontendAction_102 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_102>(TheRewriter);
    }

private:
    Rewriter TheRewriter;
    class MutatorASTConsumer_102 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_102(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) {
            //Check whether the matched AST node is the target node
            if (auto *SwitchStmt = Result.Nodes.getNodeAs<clang::SwitchStmt>("SwitchStmt")) {
              //Filter nodes in header files
              if (!SwitchStmt || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SwitchStmt->getBeginLoc()))
                return;
              //Get the source code text of target node
              auto CondVar = SwitchStmt->getCond();
              auto VarType = CondVar->getType();
              std::vector<const clang::VarDecl*> CompatibleVars;
              for (auto &D : Result.Context->getTranslationUnitDecl()->decls()) {
                if (auto VD = dyn_cast<clang::VarDecl>(&D)) {
                  if (VD->getType() == VarType) {
                    CompatibleVars.push_back(VD);
                  }
                }
              }
              for (auto &Var : CompatibleVars) {
                if (dyn_cast<clang::VarDecl>(CondVar) != Var) {
                  //Perform mutation on the source code text by applying string replacement
                  std::string Replacement = Var->getNameAsString();
                  unsigned Length = CondVar->getSourceRange().getEnd().getOffset() - CondVar->getSourceRange().getBegin().getOffset();
                  Rewrite.ReplaceText(CondVar->getSourceRange().getBegin(), Length, Replacement);
                }
              }
            }
        }
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_Substitute_Switch_Variable_102.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

// ========================================================================================================
#define MUT102_OUTPUT 1

void MutatorFrontendAction_102::MutatorASTConsumer_102::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = clang::ast_matchers::switchStmt().bind("SwitchStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}