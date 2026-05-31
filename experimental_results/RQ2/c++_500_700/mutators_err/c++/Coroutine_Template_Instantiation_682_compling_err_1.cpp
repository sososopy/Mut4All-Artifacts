//header file
#pragma once
#include "Mutator_base.h"

/**
 * coroutine_template_instantiation_682
 */ 
class MutatorFrontendAction_682 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(682)

private:
    class MutatorASTConsumer_682 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_682(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_682.h"

// ========================================================================================================
#define MUT682_OUTPUT 1

void MutatorFrontendAction_682::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("CoroutineMethod")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      if (MT->isCoroutine() && MT->getDescribedFunctionTemplate()) {
        // Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
        // Perform mutation on the source code text by applying string replacement
        std::string templateParam = MT->getDescribedFunctionTemplate()->getTemplateParameters()->getParam(0)->getNameAsString();
        std::string recursiveCall = "\n/*mut682*/anotherFunc<" + templateParam + ">();\n";
        declaration.insert(declaration.find("{") + 1, recursiveCall);
        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_682::MutatorASTConsumer_682::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(isCoroutine(), hasAncestor(functionTemplateDecl())).bind("CoroutineMethod");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}