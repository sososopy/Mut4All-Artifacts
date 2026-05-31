//header file
#pragma once
#include "Mutator_base.h"

/**
 * add_attribute_to_default_template_argument_551
 */ 
class MutatorFrontendAction_551 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(551)

private:
    class MutatorASTConsumer_551 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_551(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/add_attribute_to_default_template_argument_551.h"

// ========================================================================================================
#define MUT551_OUTPUT 1

void MutatorFrontendAction_551::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("TemplateType")) {
        //Filter nodes in header files
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (TD->hasDefaultArgument()) {
            //Get the source code text of target node
            QualType defaultArg = TD->getDefaultArgument();
            auto defaultArgRange = TD->getDefaultArgumentLoc();
            auto defaultArgText = Lexer::getSourceText(CharSourceRange::getTokenRange(defaultArgRange), *Result.SourceManager, Result.Context->getLangOpts());

            //Perform mutation on the source code text by applying string replacement
            std::string mutatedText = "[[nodiscard]] " + defaultArgText.str();

            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(defaultArgRange, mutatedText);
        }
    }
}

void MutatorFrontendAction_551::MutatorASTConsumer_551::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateTypeParmDecl(hasDefaultArgument()).bind("TemplateType");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}