// header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Requires_Clause_With_Complex_Expression_119
 */ 
class MutatorFrontendAction_119 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(119)

private:
    class MutatorASTConsumer_119 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_119(Rewriter &R) : TheRewriter(R) {}
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

// source file
#include "modify_requires_clause_with_complex_expression_119.h"

// ========================================================================================================
#define MUT119_OUTPUT 1

void MutatorFrontendAction_119::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithRequires")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->getTemplateSpecializationKind() != TSK_Undeclared && FD->getRequiresClause()) {
            auto requiresClause = FD->getRequiresClause();
            auto requiresSourceRange = requiresClause->getSourceRange();
            std::string originalRequires = stringutils::rangetoStr(*(Result.SourceManager), requiresSourceRange);

            std::string modifiedRequires = originalRequires;
            std::string additionalFunction = R"(
                template <typename U>
                auto another_function(U&& value) -> decltype(auto) {
                    return std::forward<U>(value);
                }
            )";

            size_t pos = modifiedRequires.find("decltype(");
            if (pos != std::string::npos) {
                modifiedRequires.insert(pos + 8, "another_function(");
                pos = modifiedRequires.find(")", pos);
                if (pos != std::string::npos) {
                    modifiedRequires.insert(pos, ")");
                }
            }

            Rewrite.ReplaceText(requiresSourceRange, modifiedRequires);
            Rewrite.InsertTextAfterToken(FD->getEndLoc(), additionalFunction);
        }
    }
}

void MutatorFrontendAction_119::MutatorASTConsumer_119::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl(hasRequiresClause()).bind("FunctionWithRequires");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}