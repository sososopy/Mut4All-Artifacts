//source file
#include "../include/modify_user_defined_conversion_operator_22.h"

// ========================================================================================================
#define MUT22_OUTPUT 1

void MutatorFrontendAction_22::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ConvOp = Result.Nodes.getNodeAs<clang::CXXConversionDecl>("ConversionOperator")) {
      //Filter nodes in header files
      if (!ConvOp || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ConvOp->getLocation()))
        return;

      // Get the class or struct declaration
      const auto *ParentRecord = ConvOp->getParent();
      if (!ParentRecord) return;

      // Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 ConvOp->getSourceRange());
      
      // Perform mutation on the source code text by applying string replacement
      std::string originalReturnType = ConvOp->getConversionType().getAsString();
      std::string mutatedReturnType = originalReturnType + "*";
      
      // Modify the return type in the declaration
      size_t pos = declaration.find(originalReturnType);
      if (pos != std::string::npos) {
        declaration.replace(pos, originalReturnType.length(), mutatedReturnType);
      }
      
      // Modify the body of the conversion operator
      size_t bodyStart = declaration.find("{");
      std::string bodyContent = "\n" + originalReturnType + " temp{*this};\nreturn &temp;\n";
      if (bodyStart != std::string::npos) {
        declaration.insert(bodyStart + 1, bodyContent);
      }

      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ConvOp->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_22::MutatorASTConsumer_22::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConversionDecl().bind("ConversionOperator");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}