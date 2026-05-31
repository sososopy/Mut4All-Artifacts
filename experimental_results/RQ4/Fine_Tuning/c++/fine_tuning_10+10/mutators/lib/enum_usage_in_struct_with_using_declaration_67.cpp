//source file
#include "../include/Enum_Usage_In_Struct_With_Using_Declaration_67.h"

// ========================================================================================================
#define MUT67_OUTPUT 1

void MutatorFrontendAction_67::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (auto *BD = FD->getBody()) {
            std::string bodyText = stringutils::rangetoStr(*(Result.SourceManager), BD->getSourceRange());
            size_t usingPos = bodyText.find("using enum");

            if (usingPos != std::string::npos) {
                size_t semicolonPos = bodyText.find(";", usingPos);
                if (semicolonPos != std::string::npos) {
                    std::string enumUsage = bodyText.substr(usingPos, semicolonPos - usingPos + 1);
                    std::string newExpression = "(Y & Z)"; // Example mutated expression
                    size_t exprPos = bodyText.find("int result = (X | Y);");

                    if (exprPos != std::string::npos) {
                        bodyText.replace(exprPos, std::string("int result = (X | Y);").length(), "int result = " + newExpression + ";");
                        Rewrite.ReplaceText(CharSourceRange::getTokenRange(BD->getSourceRange()), bodyText);
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_67::MutatorASTConsumer_67::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasBody(compoundStmt(hasDescendant(declStmt(hasDescendant(usingDecl())))))).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}