//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Function_With_Template_477
 */ 
class MutatorFrontendAction_477 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(477)

private:
    class MutatorASTConsumer_477 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_477(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_function_with_template_477.h"

// ========================================================================================================
#define MUT477_OUTPUT 1

void MutatorFrontendAction_477::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;
        
        if (!FD->hasBody() || FD->isTemplateInstantiation())
            return;

        auto functionName = FD->getNameAsString();
        auto returnType = FD->getReturnType().getAsString();
        auto params = FD->parameters();
        
        std::string templateDecl = "template<typename T>\n";
        std::string newFunctionDecl = returnType + " " + functionName + "(";
        for (size_t i = 0; i < params.size(); ++i) {
            if (i > 0) newFunctionDecl += ", ";
            newFunctionDecl += params[i]->getType().getAsString() + " " + params[i]->getNameAsString();
        }
        newFunctionDecl += ")";

        auto functionBody = stringutils::rangetoStr(*(Result.SourceManager), FD->getBody()->getSourceRange());
        std::string mutatedFunction = templateDecl + newFunctionDecl + functionBody;

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), mutatedFunction);
    }
}
  
void MutatorFrontendAction_477::MutatorASTConsumer_477::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(unless(cxxMethodDecl())).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}