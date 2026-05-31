//source file
#include "../include/FunctionParameterListExpansion_194.h"

// ========================================================================================================
#define MUT194_OUTPUT 1

void MutatorFrontendAction_194::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("func")) {
        // Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->getNumParams() == 0)
            return;

        // Get the first parameter's type
        QualType firstParamType = FD->getParamDecl(0)->getType();
        ASTContext &Context = *Result.Context;
        PrintingPolicy policy = Context.getPrintingPolicy();
        std::string typeName;
        {
            llvm::raw_string_ostream OS(typeName);
            firstParamType.print(OS, policy);
            OS.flush();
        }

        // Generate new parameter type (const reference)
        std::string newParamType = "const " + typeName + "&";
        std::string newParamName = "newParam_194";
        std::string newParam = ", " + newParamType + " " + newParamName;

        // Get the source code of the function's parameter list
        SourceManager &SM = Result.Context->getSourceManager();
        SourceLocation nameLoc = FD->getNameInfo().getLoc();
        SourceLocation lParenLoc = Lexer::getLocForEndOfToken(nameLoc, 1, SM, Context.getLangOpts());
        SourceLocation rParenLoc = FD->getSourceRange().getEnd();

        if (lParenLoc.isInvalid() || rParenLoc.isInvalid())
            return;

        std::string paramListText = stringutils::rangetoStr(SM, CharSourceRange::getTokenRange(lParenLoc, rParenLoc).getAsRange());

        // Insert the new parameter before the closing parenthesis
        size_t closePos = paramListText.find(')');
        if (closePos == std::string::npos)
            return;

        paramListText.insert(closePos, newParam);

        // Replace the original parameter list with the new one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(lParenLoc, rParenLoc).getAsRange(), paramListText);
    }
}
  
void MutatorFrontendAction_194::MutatorASTConsumer_194::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = functionDecl(hasAncestor(namespaceDecl(hasAncestor(namespaceDecl())))).bind("func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}