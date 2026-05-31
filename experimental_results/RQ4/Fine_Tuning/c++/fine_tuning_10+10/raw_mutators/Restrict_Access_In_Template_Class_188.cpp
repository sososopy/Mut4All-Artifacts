//header file
#pragma once
#include "Mutator_base.h"

/**
 * Restrict_Access_In_Template_Class_188
 */ 
class MutatorFrontendAction_188 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(188)

private:
    class MutatorASTConsumer_188 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_188(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Restrict_Access_In_Template_Class_188.h"

// ========================================================================================================
#define MUT188_OUTPUT 1

void MutatorFrontendAction_188::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TemplateClass")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      for (auto *D : MT->decls()) {
        if (auto *MD = llvm::dyn_cast<clang::CXXMethodDecl>(D)) {
          if (MD->getAccess() == clang::AS_public) {
            //Get the source code text of target node
            auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                       MD->getSourceRange());
            //Perform mutation on the source code text by applying string replacement
            declaration = "protected: /*mut188*/\n" + declaration;
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(MD->getSourceRange()), declaration);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_188::MutatorASTConsumer_188::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl(
        has(cxxRecordDecl(isTemplateInstantiation()).bind("TemplateClass"))
    );
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}