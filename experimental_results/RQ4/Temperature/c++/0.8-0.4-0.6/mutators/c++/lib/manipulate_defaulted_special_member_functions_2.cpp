//source file
#include "../include/manipulate_defaulted_special_member_functions_2.h"

// ========================================================================================================
#define MUT2_OUTPUT 1

void MutatorFrontendAction_2::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      
      if (MT->isDefaulted() && MT->isUserProvided()) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        std::string mutatedDeclaration = declaration;
        size_t pos = mutatedDeclaration.find("(");
        if (pos != std::string::npos) {
            mutatedDeclaration.insert(pos + 1, "this ");
        }
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
      }
    }
}
  
void MutatorFrontendAction_2::MutatorASTConsumer_2::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(isDefaulted(), isUserProvided()).bind("Methods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}