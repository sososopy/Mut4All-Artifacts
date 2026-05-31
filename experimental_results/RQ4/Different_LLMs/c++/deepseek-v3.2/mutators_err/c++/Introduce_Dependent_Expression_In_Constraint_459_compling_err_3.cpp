//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Dependent_Expression_In_Constraint_459
 */ 
class MutatorFrontendAction_459 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(459)

private:
    class MutatorASTConsumer_459 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_459(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TemplateDecl*> templateDecls;
    };
};

//source file
#include "../include/Mutator_Introduce_Dependent_Expression_In_Constraint_459.h"

// ========================================================================================================
#define MUT459_OUTPUT 1

void MutatorFrontendAction_459::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      templateDecls.push_back(TD);
    }
    else if (auto *CE = Result.Nodes.getNodeAs<clang::Expr>("ConstraintExpr")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto constraintText = stringutils::rangetoStr(*(Result.SourceManager),
                                                   CE->getSourceRange());
      //Find a suitable template declaration context
      const clang::TemplateDecl* contextTemplate = nullptr;
      for (auto TD : templateDecls) {
        if (Result.SourceManager->isBeforeInTranslationUnit(TD->getBeginLoc(), CE->getBeginLoc()) &&
            Result.SourceManager->isBeforeInTranslationUnit(CE->getEndLoc(), TD->getEndLoc())) {
          contextTemplate = TD;
          break;
        }
      }
      if (!contextTemplate) return;
      
      //Analyze the constraint expression to find non-dependent subexpressions
      //We'll look for boolean literals or integer literals
      if (constraintText.find("true") != std::string::npos) {
        //Replace 'true' with 'sizeof(T) > 0' where T is a template parameter
        auto* templ = contextTemplate->getTemplateParameters();
        if (templ && templ->size() > 0) {
          auto param = templ->getParam(0);
          std::string paramName;
          if (param->isTemplateParameter()) {
            if (auto* ttp = dyn_cast<clang::TemplateTypeParmDecl>(param)) {
              paramName = ttp->getNameAsString();
            } else if (auto* nttp = dyn_cast<clang::NonTypeTemplateParmDecl>(param)) {
              paramName = nttp->getNameAsString();
            }
          }
          if (!paramName.empty()) {
            //Perform mutation on the source code text by applying string replacement
            std::string replacement = "sizeof(" + paramName + ") > 0";
            constraintText.replace(constraintText.find("true"), 4, replacement);
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), constraintText);
          }
        }
      } else if (constraintText.find("false") != std::string::npos) {
        //Replace 'false' with 'sizeof(T) == 0' where T is a template parameter
        auto* templ = contextTemplate->getTemplateParameters();
        if (templ && templ->size() > 0) {
          auto param = templ->getParam(0);
          std::string paramName;
          if (param->isTemplateParameter()) {
            if (auto* ttp = dyn_cast<clang::TemplateTypeParmDecl>(param)) {
              paramName = ttp->getNameAsString();
            } else if (auto* nttp = dyn_cast<clang::NonTypeTemplateParmDecl>(param)) {
              paramName = nttp->getNameAsString();
            }
          }
          if (!paramName.empty()) {
            //Perform mutation on the source code text by applying string replacement
            std::string replacement = "sizeof(" + paramName + ") == 0";
            constraintText.replace(constraintText.find("false"), 5, replacement);
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), constraintText);
          }
        }
      } else if (constraintText.find("0") != std::string::npos) {
        //Replace integer literal '0' with 'sizeof(T)' where T is a template parameter
        auto* templ = contextTemplate->getTemplateParameters();
        if (templ && templ->size() > 0) {
          auto param = templ->getParam(0);
          std::string paramName;
          if (param->isTemplateParameter()) {
            if (auto* ttp = dyn_cast<clang::TemplateTypeParmDecl>(param)) {
              paramName = ttp->getNameAsString();
            } else if (auto* nttp = dyn_cast<clang::NonTypeTemplateParmDecl>(param)) {
              paramName = nttp->getNameAsString();
            }
          }
          if (!paramName.empty()) {
            //Perform mutation on the source code text by applying string replacement
            std::string replacement = "sizeof(" + paramName + ")";
            constraintText.replace(constraintText.find("0"), 1, replacement);
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), constraintText);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_459::MutatorASTConsumer_459::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher templateMatcher = clang::ast_matchers::functionTemplateDecl().bind("TemplateDecl");
    DeclarationMatcher constraintMatcher = clang::ast_matchers::expr(clang::ast_matchers::hasParent(clang::ast_matchers::requiresExpr())).bind("ConstraintExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(constraintMatcher, &callback);
    matchFinder.matchAST(Context);
}