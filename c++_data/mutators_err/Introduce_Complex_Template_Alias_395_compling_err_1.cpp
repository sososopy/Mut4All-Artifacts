//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Complex_Template_Alias_395
 */ 
class MutatorFrontendAction_395 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(395)

private:
    class MutatorASTConsumer_395 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_395(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Introduce_Complex_Template_Alias_395.h"

// ========================================================================================================
#define MUT395_OUTPUT 1

void MutatorFrontendAction_395::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      
      if (FD->getTemplatedDecl()->hasBody()) {
        //Get the source code text of target node
        auto functionBody = stringutils::rangetoStr(*(Result.SourceManager),
                                                    FD->getTemplatedDecl()->getBody()->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        std::string alias = "\nusing ComplexType = std::conditional_t<true, S<int>, std::tuple<S<int>>>;\n";
        functionBody.insert(functionBody.find("{") + 1, alias);
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getTemplatedDecl()->getBody()->getSourceRange()), functionBody);
      }
    }
}
  
void MutatorFrontendAction_395::MutatorASTConsumer_395::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl(hasTemplateParameterList(
        has(templateTypeParmDecl(hasTypeTemplateParameter(hasName("S"))))))
        .bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}