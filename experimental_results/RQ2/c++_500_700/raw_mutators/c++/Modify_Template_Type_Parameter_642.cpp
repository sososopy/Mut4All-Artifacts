//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_type_parameter_642
 */ 
class MutatorFrontendAction_642 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(642)

private:
    class MutatorASTConsumer_642 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_642(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_type_parameter_642.h"

// ========================================================================================================
#define MUT642_OUTPUT 1

void MutatorFrontendAction_642::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplate")) {
        if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FTD->getLocation()))
            return;

        auto &SM = Result.Context->getSourceManager();
        auto &LOM = Result.Context->getLangOpts();

        // Get the source code text of target node
        std::string originalText = Lexer::getSourceText(CharSourceRange::getTokenRange(FTD->getSourceRange()), SM, LOM).str();

        // Perform mutation on the source code text by applying string replacement
        std::string newText = originalText;
        size_t pos = newText.find(">");
        if (pos != std::string::npos) {
            std::string insertion = ", typename M = int ExampleStruct::*";
            newText.insert(pos, insertion);
        }

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FTD->getSourceRange()), newText);
    }
}
  
void MutatorFrontendAction_642::MutatorASTConsumer_642::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(hasTemplateParameterList(
        anyOf(hasDescendant(templateTypeParmDecl()), hasDescendant(templateTypeParmDecl())))).bind("FuncTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}