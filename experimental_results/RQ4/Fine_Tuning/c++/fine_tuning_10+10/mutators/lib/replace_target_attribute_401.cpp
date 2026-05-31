//source file
#include "../include/Replace_Target_Attribute_401.h"

// ========================================================================================================
#define MUT401_OUTPUT 1

void MutatorFrontendAction_401::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithTarget")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        for (auto *Attr : FD->specific_attrs<clang::TargetAttr>()) {
            auto originalText = Attr->getAttributeSpellingListIndex();
            if (originalText != std::string::npos) {
                std::string newText = "unsupported_arch";
                Rewrite.ReplaceText(Attr->getRange(), newText);
            }
        }
    }
}

void MutatorFrontendAction_401::MutatorASTConsumer_401::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasAttr(attr::Target)).bind("FunctionWithTarget");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}