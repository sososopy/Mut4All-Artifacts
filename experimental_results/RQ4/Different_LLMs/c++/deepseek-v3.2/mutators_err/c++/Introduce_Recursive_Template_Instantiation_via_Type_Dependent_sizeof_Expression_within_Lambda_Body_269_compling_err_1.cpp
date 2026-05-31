//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Recursive_Template_Instantiation_via_Type_Dependent_sizeof_Expression_within_Lambda_Body_269
 */ 
class MutatorFrontendAction_269 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(269)

private:
    class MutatorASTConsumer_269 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_269(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_269.h"

// ========================================================================================================
#define MUT269_OUTPUT 1

void MutatorFrontendAction_269::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      templateDecls.push_back(MT);
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto lambdaRange = MT->getSourceRange();
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager), lambdaRange);
      //Find enclosing template context
      const clang::DeclContext* context = MT->getDecl()->getDeclContext();
      const clang::TemplateDecl* enclosingTemplate = nullptr;
      while (context) {
        if (const clang::TemplateDecl* TD = dyn_cast<clang::TemplateDecl>(context)) {
          enclosingTemplate = TD;
          break;
        }
        context = context->getParent();
      }
      if (!enclosingTemplate) {
        //Check if any templateDecls are available
        if (templateDecls.empty()) return;
        size_t index = getrandom::getRandomIndex(templateDecls.size() - 1);
        enclosingTemplate = templateDecls[index];
      }
      //Determine dependent type for sizeof
      std::string dependentTypeExpr;
      const clang::ClassTemplateDecl* CTD = dyn_cast<clang::ClassTemplateDecl>(enclosingTemplate);
      const clang::FunctionTemplateDecl* FTD = dyn_cast<clang::FunctionTemplateDecl>(enclosingTemplate);
      if (CTD) {
        //Use template parameter type
        auto* TPT = CTD->getTemplateParameters()->getParam(0);
        if (TPT) {
          if (TPT->isTypeParameter()) {
            dependentTypeExpr = TPT->getNameAsString();
          } else {
            //Introduce dummy struct
            dependentTypeExpr = "Dummy";
            //Insert dummy struct definition into template
            SourceLocation insertLoc = CTD->getBeginLoc().getLocWithOffset(1);
            Rewrite.InsertTextAfterToken(insertLoc, "\nstruct Dummy {};");
          }
        }
      } else if (FTD) {
        //Use template parameter type
        auto* TPT = FTD->getTemplateParameters()->getParam(0);
        if (TPT) {
          if (TPT->isTypeParameter()) {
            dependentTypeExpr = TPT->getNameAsString();
          } else {
            dependentTypeExpr = "int";
          }
        }
      } else {
        return;
      }
      //Perform mutation on the source code text by applying string replacement
      std::string sizeofExpr = "sizeof(" + dependentTypeExpr + ");";
      if (MT->getBody()) {
        //Lambda has body, insert sizeof expression
        auto bodyRange = MT->getBody()->getSourceRange();
        auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), bodyRange);
        if (bodyText.find('{') != std::string::npos && bodyText.find('}') != std::string::npos) {
          bodyText.insert(bodyText.find('{') + 1, "\n/*mut269*/" + sizeofExpr + "\n");
          lambdaText.replace(bodyText.find(bodyText), bodyText.length(), bodyText);
        }
      } else {
        //Lambda has no body, replace with compound statement containing sizeof
        size_t lambdaEnd = lambdaText.find(')');
        if (lambdaEnd != std::string::npos) {
          lambdaText.insert(lambdaEnd + 1, "{ /*mut269*/" + sizeofExpr + " }");
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(lambdaRange), lambdaText);
    }
}
  
void MutatorFrontendAction_269::MutatorASTConsumer_269::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher templateMatcher = templateDecl().bind("TemplateDecl");
    DeclarationMatcher lambdaMatcher = lambdaExpr().bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.matchAST(Context);
}