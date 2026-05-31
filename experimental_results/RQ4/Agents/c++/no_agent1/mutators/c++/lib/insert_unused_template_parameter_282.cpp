//source file
#include "../include/insert_unused_template_parameter_282.h"

// ========================================================================================================
#define MUT282_OUTPUT 1

void MutatorFrontendAction_282::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplates")) {
        if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FT->getLocation()))
            return;

        if (FT->getTemplatedDecl()->hasBody()) {
            auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                       FT->getSourceRange());

            // Perform mutation on the source code text by applying string replacement
            std::string unusedTemplateParam = "typename UnusedParam = int";
            size_t pos = declaration.find('<');
            if (pos != std::string::npos) {
                declaration.insert(pos + 1, unusedTemplateParam + ", ");
            } else {
                // If no template parameters exist, add the whole template parameter list
                declaration.insert(0, "template <" + unusedTemplateParam + ">\n");
            }

            // Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FT->getSourceRange()), declaration);
        }
    }
}
  
void MutatorFrontendAction_282::MutatorASTConsumer_282::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = functionTemplateDecl().bind("FuncTemplates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}