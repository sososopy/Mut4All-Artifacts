//header file
#pragma once
#include "Mutator_base.h"

/**
 * Union_Empty_Array_Initialization_322
 */ 
class MutatorFrontendAction_322 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(322)

private:
    class MutatorASTConsumer_322 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_322(Rewriter &R) : TheRewriter(R) {}
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
        
    };
};

//source file
#include "../include/Union_Empty_Array_Initialization_322.h"

// ========================================================================================================
#define MUT322_OUTPUT 1

void MutatorFrontendAction_322::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Union")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (!DL->isUnion())
        return;
      auto fields = DL->fields();
      for (auto field : fields) {
        if (field->getType()->isIncompleteArrayType()) {
          auto fieldname = field->getNameAsString();
          if (fieldname == "x") {
            llvm::outs() << "find target union\n";
            auto decls = DL->decls();
            for (auto decl : decls) {
              if (decl->getIdentifierNamespace() ==
                  Decl::IdentifierNamespace::IDNS_Ordinary) {
                auto name = decl->getNameAsString();
                llvm::outs() << name << '\n';
                auto var_decl = dyn_cast<VarDecl>(decl);
                if (var_decl && var_decl->hasInit() == false) {
                  llvm::outs() << "find target var\n";
                  auto ins = "/*mut322*/ = {0}";
                  Rewrite.ReplaceText(var_decl->getEndLoc(), 0, ins);
                }
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_322::MutatorASTConsumer_322::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("Union");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}