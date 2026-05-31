//source file
#include "../include/universal_char_in_identifier_457.h"

// ========================================================================================================
#define MUT457_OUTPUT 1

void MutatorFrontendAction_457::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
            return;

        std::string originalName = VD->getNameAsString();
        if (originalName.empty())
            return;

        std::string modifiedName = originalName;
        size_t pos = modifiedName.find('V');
        if (pos != std::string::npos) {
            modifiedName.replace(pos, 1, "\\u0056");
            identifierMap[originalName] = modifiedName;

            SourceLocation startLoc = VD->getLocation();
            Rewrite.ReplaceText(startLoc, originalName.length(), modifiedName);
        }
    }

    if (auto *ID = Result.Nodes.getNodeAs<clang::DeclRefExpr>("idRef")) {
        if (!ID || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ID->getLocation()))
            return;

        std::string originalName = ID->getNameInfo().getName().getAsString();
        if (identifierMap.find(originalName) != identifierMap.end()) {
            std::string modifiedName = identifierMap[originalName];
            SourceLocation startLoc = ID->getLocation();
            Rewrite.ReplaceText(startLoc, originalName.length(), modifiedName);
        }
    }
}
  
void MutatorFrontendAction_457::MutatorASTConsumer_457::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher varDeclMatcher = varDecl().bind("varDecl");
    StatementMatcher idRefMatcher = declRefExpr().bind("idRef");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(varDeclMatcher, &callback);
    matchFinder.addMatcher(idRefMatcher, &callback);
    matchFinder.matchAST(Context);
}