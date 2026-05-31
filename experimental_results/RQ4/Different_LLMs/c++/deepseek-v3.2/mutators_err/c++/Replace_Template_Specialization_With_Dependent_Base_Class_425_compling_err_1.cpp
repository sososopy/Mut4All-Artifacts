//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Specialization_With_Dependent_Base_Class_425
 */ 
class MutatorFrontendAction_425 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(425)

private:
    class MutatorASTConsumer_425 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_425(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateDecl *> templateDecls;
    };
};

//source file
#include "../include/Mutator_425.h"

// ========================================================================================================
#define MUT425_OUTPUT 1

void MutatorFrontendAction_425::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("templateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      templateDecls.push_back(TD);
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FriendDecl>("friendDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto friendDeclText = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
      auto friendFunctionDecl = FD->getFriendDecl();
      if (!friendFunctionDecl || !isa<FunctionDecl>(friendFunctionDecl))
        return;
      auto functionDecl = dyn_cast<FunctionDecl>(friendFunctionDecl);
      auto paramType = functionDecl->getParamDecl(0)->getType();
      auto templateSpecializationType = dyn_cast<TemplateSpecializationType>(paramType);
      if (!templateSpecializationType)
        return;
      auto templateName = templateSpecializationType->getTemplateName();
      auto templateDecl = templateName.getAsTemplateDecl();
      if (!templateDecl)
        return;
      //Check if the template decl is one of the recorded class templates
      bool found = false;
      const clang::ClassTemplateDecl *targetTemplateDecl = nullptr;
      for (auto recordedDecl : templateDecls) {
        if (recordedDecl == templateDecl) {
          found = true;
          targetTemplateDecl = recordedDecl;
          break;
        }
      }
      if (!found)
        return;
      //Perform mutation on the source code text by applying string replacement
      std::string baseName = "Base";
      std::string mutatedText = friendDeclText;
      size_t pos = mutatedText.find(targetTemplateDecl->getNameAsString());
      if (pos != std::string::npos) {
        mutatedText.replace(pos, targetTemplateDecl->getNameAsString().length(), baseName);
        //Insert Base declaration before the class template if not already present
        std::string baseDecl = "template<typename... Params> struct " + baseName + " : " + targetTemplateDecl->getNameAsString() + "<Params...> {};\n";
        Rewrite.InsertTextBefore(targetTemplateDecl->getBeginLoc(), baseDecl);
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), mutatedText);
      }
    }
}
  
void MutatorFrontendAction_425::MutatorASTConsumer_425::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher templateMatcher = classTemplateDecl().bind("templateDecl");
    DeclarationMatcher friendMatcher = friendDecl(hasFriendDecl(functionDecl(hasParameter(0, parmVarDecl())))).bind("friendDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(friendMatcher, &callback);
    matchFinder.matchAST(Context);
}