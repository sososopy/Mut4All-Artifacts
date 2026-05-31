//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Constraint_With_Unsatisfiable_61
 */ 
class MutatorFrontendAction_61 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(61)

private:
    class MutatorASTConsumer_61 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_61(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Mutator_Replace_Constraint_With_Unsatisfiable_61.h"

// ========================================================================================================
#define MUT61_OUTPUT 1

void MutatorFrontendAction_61::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      
      // Get the lambda's call operator
      auto *callOp = MT->getCallOperator();
      if (!callOp) return;
      
      // Check if it has a template parameter list with a parameter pack
      auto *tpl = callOp->getTemplateSpecializationInfo();
      if (!tpl) return;
      
      auto *templateDecl = tpl->getTemplate();
      if (!templateDecl) return;
      
      // Look for parameter pack in template parameters
      bool hasPack = false;
      for (auto *param : *templateDecl->getTemplateParameters()) {
        if (auto *ttp = dyn_cast<TemplateTypeParmDecl>(param)) {
          if (ttp->isParameterPack()) {
            hasPack = true;
            break;
          }
        }
      }
      if (!hasPack) return;
      
      // Get the lambda's parameter list
      auto paramList = callOp->parameters();
      if (paramList.empty()) return;
      
      // Check if first parameter has a constraint
      auto *firstParam = paramList[0];
      auto constraints = firstParam->getTrailingRequiresClause();
      if (!constraints) return;
      
      // Get the constraint expression source range
      if (!constraints) return;
      
      // Get the source code of the constraint
      SourceManager &SM = Rewrite.getSourceMgr();
      LangOptions LangOpts;
      std::string constraintText = stringutils::rangetoStr(SM, constraints->getSourceRange());
      
      // Check if it contains std::same_as
      if (constraintText.find("std::same_as") == std::string::npos) return;
      
      // Perform mutation: replace with unsatisfiable constraint
      // First, ensure always_false trait is available
      std::string traitDef = "\ntemplate<typename... Ts> struct always_false_61 { static constexpr bool value = false; };\n";
      
      // Insert the trait definition at the beginning of the translation unit if not already present
      SourceLocation insertLoc = SM.getLocForStartOfFile(SM.getMainFileID());
      CharSourceRange range = CharSourceRange::getCharRange(insertLoc, insertLoc.getLocWithOffset(100));
      std::string existingText = stringutils::rangetoStr(SM, range.getAsRange());
      if (existingText.find("always_false_61") == std::string::npos) {
        Rewrite.InsertText(insertLoc, traitDef);
      }
      
      // Create the new constraint
      std::string newConstraint = "requires always_false_61<Ts...>::value";
      
      // Replace the old constraint with the new one
      Rewrite.ReplaceText(constraints->getSourceRange(), newConstraint);
    }
}
  
void MutatorFrontendAction_61::MutatorASTConsumer_61::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr(hasDescendant(requiresClause().bind("requiresClause"))).bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}