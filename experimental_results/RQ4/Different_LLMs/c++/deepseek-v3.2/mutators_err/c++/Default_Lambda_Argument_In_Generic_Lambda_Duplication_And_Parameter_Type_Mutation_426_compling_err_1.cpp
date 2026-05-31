//header file
#pragma once
#include "Mutator_base.h"

/**
 * Default_Lambda_Argument_In_Generic_Lambda_Duplication_And_Parameter_Type_Mutation_426
 */ 
class MutatorFrontendAction_426 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(426)

private:
    class MutatorASTConsumer_426 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_426(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
        std::map<std::string, int> nameCounts;
    };
};

//source file
#include "../include/Default_Lambda_Argument_In_Generic_Lambda_Duplication_And_Parameter_Type_Mutation_426.h"

// ========================================================================================================
#define MUT426_OUTPUT 1

void MutatorFrontendAction_426::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LD = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!LD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LD->getLocation()))
        return;
      
      const clang::CXXRecordDecl *LambdaClass = LD->getLambdaClass();
      if (!LambdaClass) return;
      
      clang::FunctionDecl *CallOperator = LambdaClass->getLambdaCallOperator();
      if (!CallOperator) return;
      
      // Check if lambda is generic (has auto parameters)
      bool hasAutoParam = false;
      bool hasDefaultArg = false;
      int defaultArgParamIndex = -1;
      for (unsigned i = 0; i < CallOperator->getNumParams(); ++i) {
        clang::ParmVarDecl *Param = CallOperator->getParamDecl(i);
        if (Param->getType()->isAutoType()) {
          hasAutoParam = true;
        }
        if (Param->hasDefaultArg()) {
          hasDefaultArg = true;
          defaultArgParamIndex = i;
        }
      }
      
      if (!hasAutoParam || !hasDefaultArg || defaultArgParamIndex <= 0) return;
      
      //Get the source code text of target node
      std::string lambdaText = stringutils::rangetoStr(*(Result.SourceManager), LD->getSourceRange());
      
      // Find the variable name if lambda is assigned to a variable
      std::string varName = "";
      if (const clang::Decl *ParentDecl = Result.Nodes.getNodeAs<clang::Decl>("parentDecl")) {
        if (const clang::VarDecl *Var = clang::dyn_cast<clang::VarDecl>(ParentDecl)) {
          varName = Var->getNameAsString();
        }
      }
      
      // Perform mutation on the source code text
      // First, modify the original lambda: add const to the parameter before the one with default argument
      std::string modifiedLambda = lambdaText;
      // Find the parameter list range
      size_t paramStart = modifiedLambda.find('(');
      if (paramStart == std::string::npos) return;
      size_t paramEnd = modifiedLambda.find(')', paramStart);
      if (paramEnd == std::string::npos) return;
      
      std::string paramList = modifiedLambda.substr(paramStart + 1, paramEnd - paramStart - 1);
      // Split parameters by commas (simple approach)
      std::vector<std::string> params;
      size_t pos = 0;
      while (pos < paramList.length()) {
        size_t commaPos = paramList.find(',', pos);
        if (commaPos == std::string::npos) {
          params.push_back(paramList.substr(pos));
          break;
        }
        params.push_back(paramList.substr(pos, commaPos - pos));
        pos = commaPos + 1;
      }
      
      if (params.size() > defaultArgParamIndex) {
        // Modify the parameter at index defaultArgParamIndex - 1 (the one before default arg)
        std::string &targetParam = params[defaultArgParamIndex - 1];
        // Insert "const" before "auto"
        size_t autoPos = targetParam.find("auto");
        if (autoPos != std::string::npos) {
          targetParam.insert(autoPos, "const ");
        }
      }
      
      // Reconstruct parameter list
      std::string newParamList;
      for (size_t i = 0; i < params.size(); ++i) {
        newParamList += params[i];
        if (i < params.size() - 1) newParamList += ", ";
      }
      
      modifiedLambda.replace(paramStart + 1, paramEnd - paramStart - 1, newParamList);
      
      // Generate duplicate lambda name
      std::string dupName = varName + "_dup";
      if (nameCounts.find(dupName) != nameCounts.end()) {
        int count = nameCounts[dupName] + 1;
        nameCounts[dupName] = count;
        dupName = varName + "_dup_" + std::to_string(count);
      } else {
        nameCounts[dupName] = 1;
      }
      
      // Create duplicate lambda (original unchanged version)
      std::string dupLambda;
      if (varName.empty()) {
        dupLambda = lambdaText;
      } else {
        dupLambda = "auto " + dupName + " = " + lambdaText;
      }
      
      // Combine mutated original and duplicate
      std::string finalText;
      if (varName.empty()) {
        finalText = modifiedLambda + "; " + dupLambda + ";";
      } else {
        finalText = modifiedLambda + "; " + dupLambda + ";";
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LD->getSourceRange()), finalText);
    }
}
  
void MutatorFrontendAction_426::MutatorASTConsumer_426::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr(hasParent(varDecl().bind("parentDecl"))).bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}