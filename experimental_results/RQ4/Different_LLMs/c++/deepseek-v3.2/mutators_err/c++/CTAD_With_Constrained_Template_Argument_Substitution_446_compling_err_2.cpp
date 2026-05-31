//header file
#pragma once
#include "Mutator_base.h"

/**
 * CTAD_With_Constrained_Template_Argument_Substitution_446
 */ 
class MutatorFrontendAction_446 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(446)

private:
    class MutatorASTConsumer_446 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_446(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/CTAD_With_Constrained_Template_Argument_Substitution_446.h"

// ========================================================================================================
#define MUT446_OUTPUT 1

void MutatorFrontendAction_446::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto callExpr = MT;
      auto callee = callExpr->getCallee();
      if (!callee)
        return;
      auto declRefExpr = dyn_cast<DeclRefExpr>(callee);
      if (!declRefExpr)
        return;
      auto functionDecl = dyn_cast<FunctionDecl>(declRefExpr->getDecl());
      if (!functionDecl || !functionDecl->isTemplateInstantiation())
        return;
      auto templateDecl = functionDecl->getTemplateInstantiationPattern();
      if (!templateDecl)
        return;
      auto args = callExpr->getArgs();
      for (unsigned i = 0; i < callExpr->getNumArgs(); ++i) {
        auto arg = args[i];
        auto argType = arg->getType();
        if (argType.isNull())
          continue;
        auto templateSpecializationType = dyn_cast<TemplateSpecializationType>(argType.getTypePtr());
        if (!templateSpecializationType)
          continue;
        auto templateDecl = templateSpecializationType->getTemplateName().getAsTemplateDecl();
        if (!templateDecl)
          continue;
        auto classTemplateDecl = dyn_cast<ClassTemplateDecl>(templateDecl);
        if (!classTemplateDecl)
          continue;
        auto templateParams = classTemplateDecl->getTemplateParameters();
        bool hasConstrainedParam = false;
        for (unsigned k = 0; k < templateParams->size(); ++k) {
          auto param = templateParams->getParam(k);
          if (param->hasAssociatedConstraints()) {
            hasConstrainedParam = true;
            break;
          }
        }
        if (!hasConstrainedParam)
          continue;
        auto sourceRange = arg->getSourceRange();
        auto originalText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
        auto templateArgs = templateSpecializationType->template_arguments();
        std::string newText = classTemplateDecl->getNameAsString();
        auto constructExpr = dyn_cast<CXXConstructExpr>(arg);
        if (constructExpr) {
          auto constructorArgs = constructExpr->getArgs();
          unsigned numArgs = constructExpr->getNumArgs();
          if (numArgs > 0) {
            newText += "(";
            for (unsigned j = 0; j < numArgs; ++j) {
              auto constructorArgText = stringutils::rangetoStr(*(Result.SourceManager), constructorArgs[j]->getSourceRange());
              newText += constructorArgText;
              if (j < numArgs - 1)
                newText += ", ";
            }
            newText += ")";
          } else {
            newText += "{}";
          }
        } else {
          newText = originalText;
        }
        //Perform mutation on the source code text by applying string replacement
        if (newText != originalText) {
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(sourceRange, newText);
        }
      }
    }
}
  
void MutatorFrontendAction_446::MutatorASTConsumer_446::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = callExpr().bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}