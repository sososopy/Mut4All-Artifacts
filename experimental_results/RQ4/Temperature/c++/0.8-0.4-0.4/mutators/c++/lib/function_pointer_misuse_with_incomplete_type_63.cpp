//source file
#include "../include/function_pointer_misuse_with_incomplete_type_63.h"

// ========================================================================================================
#define MUT63_OUTPUT 1

void MutatorFrontendAction_63::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->hasBody()) {
            auto functionText = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
            std::string forwardDecl = "struct IncompleteStruct;\n";
            std::string pointerType = "IncompleteStruct *(*funcPtr)(int)";
            std::string newSignature = FD->getNameAsString() + "(" + pointerType + ")";
            
            auto startPos = functionText.find(FD->getNameAsString());
            if (startPos != std::string::npos) {
                functionText.replace(startPos, FD->getNameAsString().length(), newSignature);
                functionText = forwardDecl + functionText;
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), functionText);
            }
        }
    }
}

void MutatorFrontendAction_63::MutatorASTConsumer_63::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}