//source file
#include "../include/Modify_Nested_Array_Initialization_476.h"

// ========================================================================================================
#define MUT476_OUTPUT 1

void MutatorFrontendAction_476::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("arrayVarDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      
      if (const clang::ConstantArrayType *CAT = Result.Context->getAsConstantArrayType(VD->getType())) {
        //Get the source code text of target node
        auto arraySource = stringutils::rangetoStr(*(Result.SourceManager), VD->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        if (CAT->getElementType()->isArrayType()) {
          // Reduce dimensionality
          std::string newArray = "/*mut476*/" + arraySource.substr(0, arraySource.find('[')) + "[" + std::to_string(CAT->getSize().getZExtValue()) + "] = {0};";
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), newArray);
        } else {
          // Increase dimensionality
          std::string newArray = "/*mut476*/" + arraySource.substr(0, arraySource.find('[')) + "[" + std::to_string(CAT->getSize().getZExtValue()) + "][2] = {{0}};";
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), newArray);
        }
      }
    }
}
  
void MutatorFrontendAction_476::MutatorASTConsumer_476::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(constantArrayType())).bind("arrayVarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}