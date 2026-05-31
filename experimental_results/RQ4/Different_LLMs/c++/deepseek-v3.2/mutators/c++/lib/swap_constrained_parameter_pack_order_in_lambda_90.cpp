//source file
#include "../include/Swap_Constrained_Parameter_Pack_Order_In_Lambda_90.h"

// ========================================================================================================
#define MUT90_OUTPUT 1

void MutatorFrontendAction_90::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto lambdaDecl = MT->getCallOperator();
      if (!lambdaDecl) return;
      
      auto paramList = lambdaDecl->parameters();
      if (paramList.size() < 2) return;
      
      //Check if the lambda is inside a template
      auto parent = dyn_cast_or_null<FunctionDecl>(lambdaDecl->getParent());
      if (!parent || !parent->isTemplateInstantiation()) return;
      
      //Identify constrained parameter packs and unconstrained parameters
      std::vector<ParmVarDecl*> constrainedPacks;
      std::vector<ParmVarDecl*> unconstrainedParams;
      std::vector<ParmVarDecl*> unconstrainedPacks;
      
      for (auto param : paramList) {
        if (param->getTypeSourceInfo()) {
          auto type = param->getTypeSourceInfo()->getType();
          if (type->isTemplateTypeParmType()) {
            if (type->getContainedAutoType()) {
              //Check if it's a constrained parameter pack
              if (param->isParameterPack()) {
                constrainedPacks.push_back(param);
              } else {
                unconstrainedParams.push_back(param);
              }
            } else if (param->isParameterPack()) {
              unconstrainedPacks.push_back(param);
            }
          }
        }
      }
      
      //Ensure we have at least one constrained pack followed by unconstrained parameters
      if (constrainedPacks.empty() || unconstrainedParams.empty()) return;
      
      //Get the source code text of the parameter list
      SourceRange paramRange = paramList.front()->getSourceRange();
      paramRange.setEnd(paramList.back()->getSourceRange().getEnd());
      auto paramText = stringutils::rangetoStr(*(Result.SourceManager), paramRange);
      
      //Perform mutation on the source code text by applying string replacement
      //Swap order: constrained pack -> unconstrained param -> unconstrained pack
      std::string newParamText;
      if (!unconstrainedParams.empty()) {
        newParamText = unconstrainedParams.front()->getNameAsString() + ", ";
      }
      if (!unconstrainedPacks.empty()) {
        newParamText += unconstrainedPacks.front()->getNameAsString() + "...";
        if (!constrainedPacks.empty()) newParamText += ", ";
      }
      if (!constrainedPacks.empty()) {
        newParamText += constrainedPacks.front()->getNameAsString() + "...";
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(paramRange, newParamText);
    }
}
  
void MutatorFrontendAction_90::MutatorASTConsumer_90::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr(hasDescendant(parmVarDecl(hasType(templateTypeParmType())))).bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}