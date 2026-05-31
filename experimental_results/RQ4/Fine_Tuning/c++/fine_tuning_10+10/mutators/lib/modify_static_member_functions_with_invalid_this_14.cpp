//source file
#include "../include/modify_static_member_functions_with_invalid_this_14.h"

// ========================================================================================================
#define MUT14_OUTPUT 1

void MutatorFrontendAction_14::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("StaticMethod")) {
        //Filter nodes in header files
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;

        if (MT->isStatic() && MT->doesThisDeclarationHaveABody()) {
            //Get the source code text of target node
            auto noexceptExpr = MT->getExceptionSpecSourceRange();
            if (noexceptExpr.isValid()) {
                std::string sourceText = Lexer::getSourceText(CharSourceRange::getTokenRange(noexceptExpr), *Result.SourceManager, Result.Context->getLangOpts()).str();
                //Perform mutation on the source code text by applying string replacement
                size_t pos = sourceText.find("this->");
                if (pos != std::string::npos) {
                    sourceText.replace(pos, 5, "instance.");
                    //Replace the original AST node with the mutated one
                    Rewrite.ReplaceText(noexceptExpr, sourceText);
                }
            }
        }
    }
}

void MutatorFrontendAction_14::MutatorASTConsumer_14::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(isStaticStorageClass(), isTemplateInstantiation()).bind("StaticMethod");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}