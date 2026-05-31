//header file
#pragma once
#include "Mutator_base.h"

/**
 * Change_Typedef_To_Consteval_Template_76
 */ 
class MutatorFrontendAction_76 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(76)

private:
    class MutatorASTConsumer_76 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_76(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Change_Typedef_To_Consteval_Template_76.h"

// ========================================================================================================
#define MUT76_OUTPUT 1

void MutatorFrontendAction_76::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::TypedefDecl>("Typedefs")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      auto typedef_name = MT->getNameAsString();
      llvm::outs() << typedef_name << '\n';
      auto new_typedef = "typedef struct0<> " + typedef_name;
      Rewrite.ReplaceText(MT->getBeginLoc(), 0, "/*mut76*/");
      Rewrite.ReplaceText(MT->getSourceRange(), new_typedef);
    }
}
  
void MutatorFrontendAction_76::MutatorASTConsumer_76::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typedefDecl(hasParent(namespaceDecl())).bind("Typedefs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}