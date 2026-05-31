//source file
#include "../include/Add_Target_Version_Attribute_401.h"

// ========================================================================================================
#define MUT401_OUTPUT 1

void MutatorFrontendAction_401::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionDecl>("func")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;

        bool hasTargetAttr = false;
        for (const auto *A : MT->getAttrs()) {
            if (A->getAttrName()->getName() == "target" || 
                A->getAttrName()->getName() == "target_version") {
                hasTargetAttr = true;
                break;
            }
        }
        if (hasTargetAttr)
            return;

        std::vector<std::string> features = {"avx2", "neon", "sve", "aes", "fma"};
        std::string feature = features[getrandom::getRandomIndex(features.size())];
        std::string attr = "__attribute__((target_version(\"" + feature + "\")))";

        SourceLocation loc = MT->getSourceRange().getBegin();
        Rewrite.InsertText(loc, attr + " ", 0, true);
    }
}

void MutatorFrontendAction_401::MutatorASTConsumer_401::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl().bind("func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}