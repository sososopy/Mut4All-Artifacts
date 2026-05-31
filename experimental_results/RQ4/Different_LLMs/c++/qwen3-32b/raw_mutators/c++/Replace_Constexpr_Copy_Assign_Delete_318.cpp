//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Constexpr_Copy_Assign_Delete_318
 */ 
class MutatorFrontendAction_318 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(318)
private:
    class MutatorASTConsumer_318 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_318(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Constexpr_Copy_Assign_Delete_318.h"

// ========================================================================================================
#define MUT318_OUTPUT 1

void MutatorFrontendAction_318::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("copyAssign")) {
        if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(MD->getLocation()))
            return;

        // Verify this is a copy assignment operator
        if (!MD->getASTContext().isCopyAssignmentOperator(MD))
            return;

        // Skip if already constexpr
        if (MD->isConstexpr())
            return;

        // Get original source text
        auto SM = Result.SourceManager;
        auto SR = MD->getSourceRange();
        std::string original = stringutils::rangetoStr(*SM, SR);

        // Construct mutated text
        std::string mutated = "constexpr " + original;
        size_t semicolon = mutated.find(';');
        if (semicolon != std::string::npos) {
            mutated.replace(semicolon, 1, " = delete;");
        }

        // Apply mutation
        Rewrite.ReplaceText(SR, mutated);
    }
}
  
void MutatorFrontendAction_318::MutatorASTConsumer_318::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = 
        cxxMethodDecl(
            hasName("operator="),
            parameterCountIs(1),
            hasSingleParameterType(
                referenceType(
                    pointee(constType())
                )
            ),
            unless(isConstexpr())
        ).bind("copyAssign");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}