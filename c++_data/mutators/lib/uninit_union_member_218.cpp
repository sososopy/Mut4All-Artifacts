//source file
#include "../include/uninit_union_member_218.h"

// ========================================================================================================
#define MUT218_OUTPUT 1

void MutatorFrontendAction_218::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("UnionDecl")) {
        if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(UD->getLocation()))
            return;

        for (auto *field : UD->fields()) {
            if (field->hasInClassInitializer()) {
                // Get the source code text of target node
                auto initRange = field->getInClassInitializer()->getSourceRange();
                // Perform mutation on the source code text by applying string replacement
                Rewrite.RemoveText(initRange);
                break; // Only mutate the first member with an initializer
            }
        }
    }
}

void MutatorFrontendAction_218::MutatorASTConsumer_218::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = cxxRecordDecl(isUnion()).bind("UnionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}