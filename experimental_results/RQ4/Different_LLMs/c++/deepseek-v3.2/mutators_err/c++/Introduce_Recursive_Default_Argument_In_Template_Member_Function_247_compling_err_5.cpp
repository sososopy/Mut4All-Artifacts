//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Recursive_Default_Argument_In_Template_Member_Function_247
 */ 
class MutatorFrontendAction_247 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(247)

private:
    class MutatorASTConsumer_247 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_247(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXMethodDecl*> candidateMethods;
    };
};

//source file
#include "../include/Mutator_247.h"

// ========================================================================================================
#define MUT247_OUTPUT 1

void MutatorFrontendAction_247::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("TemplateMemberFunction")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      if (!MT->getParent()->isTemplateDecl())
        return;
      if (MT->getNumParams() == 0)
        return;
      //Record candidate methods
      candidateMethods.push_back(MT);
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("HelperFunction")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Check if helper function exists
      std::string funcName = FD->getNameAsString();
      if (funcName == "helper") {
        //Helper function already exists, no need to add
        return;
      }
    }
    else if (auto *TD = Result.Nodes.getNodeAs<clang::TranslationUnitDecl>("TranslationUnit")) {
      if (!TD)
        return;
      //Perform mutation on the source code text by applying string replacement
      if (candidateMethods.empty())
        return;
      
      //Select a random candidate method
      size_t idx = getrandom::getRandomIndex(candidateMethods.size() - 1);
      const CXXMethodDecl* method = candidateMethods[idx];
      
      //Get first parameter
      const ParmVarDecl* param = method->getParamDecl(0);
      std::string paramName = param->getNameAsString();
      if (paramName.empty()) {
        paramName = "param";
      }
      
      //Check if helper function exists in the2 context
      bool helperExists = false;
      for (auto decl : TD->decls()) {
        if (auto* funcDecl = dyn_cast<FunctionDecl>(decl)) {
          if (funcDecl->getNameAsString() == "helper") {
            helperExists = true;
            break;
          }
        }
      }
      
      //Add helper function declaration if needed
      if (!helperExists) {
        std::string helperDecl = "\n/*mut247*/int helper(int);\n";
        Rewrite.InsertTextBefore(TD->getBeginLoc(), helperDecl);
      }
      
      //Get method source range
      SourceRange range = method->getSourceRange();
      std::string methodText = stringutils::rangetoStr(*Result.SourceManager, range);
      
      //Find parameter list location
      size_t paramStart = methodText.find('(');
      if (paramStart == std::string::npos)
        return;
      
      size_t paramEnd = methodText.find(')', paramStart);
      if (paramEnd == std::string::npos)
        return;
      
      std::string paramList = methodText.substr(paramStart + 1, paramEnd - paramStart - 1);
      
      //Find the parameter declaration
      size_t paramPos = paramList.find(paramName);
      if (paramPos == std::string::npos)
        return;
      
      //Check if parameter already has default argument
      size_t equalsPos = paramList.find('=', paramPos);
      if (equalsPos != std::string::npos) {
        //Already has default argument, skip
        return;
      }
      
      //Insert default argument
      std::string newDefault = " = helper(" + paramName + ")()";
      std::string newMethodText = methodText;
      newMethodText.insert(paramEnd,1, newDefault);
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(range), newMethodText);
    }
}
  
void MutatorFrontendAction_247::MutatorASTConsumer_247::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher templateMethodMatcher = cxxMethodDecl(isTemplate(), unless(hasDefaultArg(0))).bind("TemplateMemberFunction");
    DeclarationMatcher helperMatcher = functionDecl(hasName("helper")).bind("HelperFunction");
    DeclarationMatcher tuMatcher = translationUnitDecl().bind("TranslationUnit");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMethodMatcher, &callback);
    matchFinder.addMatcher(helperMatcher, &callback);
    matchFinder.addMatcher(tuMatcher, &callback);
    matchFinder.matchAST(Context);
}