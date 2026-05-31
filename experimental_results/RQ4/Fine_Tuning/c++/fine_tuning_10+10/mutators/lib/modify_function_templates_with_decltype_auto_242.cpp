//source file
#include "../include/Modify_Function_Templates_With_Decltype_Auto_242.h"

// ========================================================================================================
#define MUT242_OUTPUT 1

void MutatorFrontendAction_242::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
        // Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        // Check if the return type is decltype(auto)
        if (FD->getReturnType()->isUndeducedType()) {
            // Get the source code text of target node
            auto declaration = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());

            // Perform mutation on the source code text by applying string replacement
            // Replace decltype(auto) with a template parameter type T
            std::string newReturnType = "T";
            size_t pos = declaration.find("decltype(auto)");
            if (pos != std::string::npos) {
                declaration.replace(pos, std::string("decltype(auto)").length(), newReturnType);
            }

            // Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
        }
    }
}

void MutatorFrontendAction_242::MutatorASTConsumer_242::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = functionDecl(isTemplateInstantiation(), returns(autoType())).bind("FuncDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}