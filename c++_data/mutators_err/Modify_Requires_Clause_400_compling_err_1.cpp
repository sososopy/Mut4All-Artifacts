//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Requires_Clause_400
 */ 
class MutatorFrontendAction_400 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(400)

private:
    class MutatorASTConsumer_400 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_400(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_requires_clause_400.h"

// ========================================================================================================
#define MUT400_OUTPUT 1

void MutatorFrontendAction_400::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithRequires")) {
        // Filter nodes in header files
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                MT->getLocation()))
            return;

        // Check if the function is a member function of a class template
        if (MT->isTemplated() && MT->getDescribedFunctionTemplate()) {
            // Get the source code text of target node
            auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                       MT->getSourceRange());

            // Perform mutation on the source code text by applying string replacement
            // Modify the requires clause logic
            size_t requiresPos = declaration.find("requires");
            if (requiresPos != std::string::npos) {
                size_t clauseStart = declaration.find("(", requiresPos);
                size_t clauseEnd = declaration.find(")", clauseStart);
                if (clauseStart != std::string::npos && clauseEnd != std::string::npos) {
                    std::string originalClause = declaration.substr(clauseStart, clauseEnd - clauseStart + 1);
                    std::string mutatedClause = "(sizeof(T) >= 1)";
                    declaration.replace(clauseStart, clauseEnd - clauseStart + 1, mutatedClause);
                }
            }

            // Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
        }
    }
}

void MutatorFrontendAction_400::MutatorASTConsumer_400::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasRequiresClause()).bind("FunctionWithRequires");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}