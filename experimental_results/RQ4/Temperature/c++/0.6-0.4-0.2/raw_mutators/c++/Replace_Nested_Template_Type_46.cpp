//header file
#pragma once
#include "Mutator_base.h"

/**
 * replace_nested_template_type_46
 */ 
class MutatorFrontendAction_46 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(46)

private:
    class MutatorASTConsumer_46 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_46(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_nested_template_type_46.h"

// ========================================================================================================
#define MUT46_OUTPUT 1

void MutatorFrontendAction_46::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NT = Result.Nodes.getNodeAs<clang::NestedNameSpecifier>("nestedType")) {
        if (!NT || !Result.Context->getSourceManager().isWrittenInMainFile(NT->getBeginLoc()))
            return;

        auto sourceRange = NT->getSourceRange();
        auto sourceText = Lexer::getSourceText(CharSourceRange::getTokenRange(sourceRange), 
                                               Result.Context->getSourceManager(), 
                                               Result.Context->getLangOpts());

        std::string mutatedText = sourceText.str() + "::nonexistent_type";
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
    }
}
  
void MutatorFrontendAction_46::MutatorASTConsumer_46::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = nestedNameSpecifier(hasType(namedDecl())).bind("nestedType");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}