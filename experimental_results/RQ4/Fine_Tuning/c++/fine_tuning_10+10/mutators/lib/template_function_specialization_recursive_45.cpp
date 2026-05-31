//source file
#include "../include/template_function_specialization_recursive_45.h"

// ========================================================================================================
#define MUT45_OUTPUT 1

void MutatorFrontendAction_45::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        
        // Get the source code text of target node
        auto funcDecl = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());

        // Perform mutation on the source code text by applying string replacement
        std::string newTemplate = "template <typename T, int N>\n";
        newTemplate += "void " + FD->getNameAsString() + "(T data) {\n";
        newTemplate += "    if constexpr (N > 0) {\n";
        newTemplate += "        " + FD->getNameAsString() + "<T, N-1>(data);\n";
        newTemplate += "    }\n";
        newTemplate += "    // Additional processing logic\n";
        newTemplate += "}\n";

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), newTemplate);
    }
}
  
void MutatorFrontendAction_45::MutatorASTConsumer_45::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}