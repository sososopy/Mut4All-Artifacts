//source file
#include "../include/Modify_Array_Initialization_With_Brace_142.h"

// ========================================================================================================
#define MUT142_OUTPUT 1

void MutatorFrontendAction_142::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("arrayVar")) {
        //Filter nodes in header files
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       VD->getLocation()))
            return;

        if (auto *CXXConstructExpr = dyn_cast<clang::CXXConstructExpr>(VD->getInit())) {
            //Get the source code text of target node
            auto sourceRange = CXXConstructExpr->getParenOrBraceRange();
            auto initExpr = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);

            //Perform mutation on the source code text by applying string replacement
            std::string mutatedInitExpr = "{" + initExpr.substr(1, initExpr.size() - 2) + "}";

            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedInitExpr);
        }
    }
}

void MutatorFrontendAction_142::MutatorASTConsumer_142::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(typedefType(hasDeclaration(typedefDecl(hasType(arrayType()))))), 
                                         hasInitializer(cxxConstructExpr())).bind("arrayVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}