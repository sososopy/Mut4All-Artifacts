//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Invalid_Concept_Pack_Expansion_314
 */ 
class MutatorFrontendAction_Replace_Invalid_Concept_Pack_Expansion_314 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(Replace_Invalid_Concept_Pack_Expansion_314)

private:
    class MutatorASTConsumer_Replace_Invalid_Concept_Pack_Expansion_314 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Replace_Invalid_Concept_Pack_Expansion_314(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Replace_Invalid_Concept_Pack_Expansion_314.h"

// ========================================================================================================
#define MUT314_OUTPUT 1

void MutatorFrontendAction_Replace_Invalid_Concept_Pack_Expansion_314::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambda")) {
        if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(LE->getBeginLoc()))
            return;

        if (auto *RC = LE->getRequiresClause()) {
            SourceLocation Start = RC->getRequiresLoc();
            SourceLocation End = RC->getRParenLoc();
            std::string requiresText = stringutils::rangetoStr(*Result.SourceManager, CharSourceRange::getCharRange(Start, End));

            size_t pos = requiresText.find("ValidConcept<");
            if (pos != std::string::npos) {
                std::string replacement = "InvalidConcept<T::value..., ";
                requiresText.replace(pos, 13, replacement);
                Rewrite.ReplaceText(CharSourceRange::getCharRange(Start, End), requiresText);
            }
        }
    }
}
  
void MutatorFrontendAction_Replace_Invalid_Concept_Pack_Expansion_314::MutatorASTConsumer_Replace_Invalid_Concept_Pack_Expansion_314::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = lambdaExpr(
        hasRequiresClause(requiresExpr().bind("requiresClause")),
        hasLambdaExprTemplateParams(
            templateParameterList(
                hasTemplateParams(
                    anyOf(
                        classTemplateParameter().bind("typeParam"),
                        nonTypeTemplateParameter()
                    )
                )
            )
        )
    ).bind("lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}