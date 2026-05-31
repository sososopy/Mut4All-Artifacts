//source file
#include "../include/template_parameter_mismatch_1.h"

// ========================================================================================================
#define MUT1_OUTPUT 1

void MutatorFrontendAction_1::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionCall")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (auto *TAL = FD->getTemplateSpecializationArgs()) {
        if (targetTemplate && TAL->size() == targetTemplate->getTemplateParameters()->size()) {
            //Perform mutation on the source code text by applying string replacement
            std::string newArgs;
            for (unsigned i = 0; i < TAL->size(); ++i) {
                newArgs += TAL->get(i).getAsType().getAsString();
                newArgs += ", ";
            }
            newArgs += "int"; // Add an additional valid type

            auto callRange = FD->getSourceRange();
            std::string originalCall = clang::Lexer::getSourceText(clang::CharSourceRange::getTokenRange(callRange), 
                                                                   *Result.SourceManager, clang::LangOptions()).str();

            std::string mutatedCall = originalCall;
            size_t angleBracketPos = mutatedCall.find('<');
            if (angleBracketPos != std::string::npos) {
                size_t closingAngleBracketPos = mutatedCall.find('>', angleBracketPos);
                if (closingAngleBracketPos != std::string::npos) {
                    mutatedCall.replace(angleBracketPos + 1, closingAngleBracketPos - angleBracketPos - 1, newArgs);
                }
            }

            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(callRange), mutatedCall);
        }
      }
    }
}
  
void MutatorFrontendAction_1::MutatorASTConsumer_1::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation
    DeclarationMatcher templateMatcher = functionTemplateDecl().bind("FunctionTemplate");
    DeclarationMatcher callMatcher = functionDecl(hasDescendant(callExpr())).bind("FunctionCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(callMatcher, &callback);
    matchFinder.matchAST(Context);
}