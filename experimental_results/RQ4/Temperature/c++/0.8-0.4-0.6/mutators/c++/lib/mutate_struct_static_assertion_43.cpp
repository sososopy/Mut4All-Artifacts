//source file
#include "../include/Mutator_43.h"

// ========================================================================================================
#define MUT43_OUTPUT 1

void MutatorFrontendAction_43::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *StaticAssertDecl = Result.Nodes.getNodeAs<clang::StaticAssertDecl>("StaticAssert")) {
        if (!StaticAssertDecl || !Result.Context->getSourceManager().isWrittenInMainFile(
                StaticAssertDecl->getLocation()))
            return;

        auto conditionRange = StaticAssertDecl->getAssertExpr()->getSourceRange();
        auto conditionText = stringutils::rangetoStr(*(Result.SourceManager), conditionRange);

        // Perform mutation on the condition text
        std::string mutatedCondition = conditionText;
        size_t pos = mutatedCondition.find("int");
        if (pos != std::string::npos) {
            mutatedCondition.replace(pos, 3, "undefinedType");
        }

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(conditionRange), mutatedCondition);
    }
}

void MutatorFrontendAction_43::MutatorASTConsumer_43::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = staticAssertDecl().bind("StaticAssert");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}