//header file
#pragma once
#include "Mutator_base.h"

/**
 * Variadic_Template_Argument_Expansion_In_Using_Declaration_225
 */ 
class MutatorFrontendAction_225 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(225)

private:
    class MutatorASTConsumer_225 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_225(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Variadic_Template_Argument_Expansion_In_Using_Declaration_225.h"

// ========================================================================================================
#define MUT225_OUTPUT 1

void MutatorFrontendAction_225::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *UD = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("UsingDecl")) {
      //Filter nodes in header files
      if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UD->getLocation()))
        return;
      //Get the source code text of target node
      auto *TAD = dyn_cast<clang::TypeAliasTemplateDecl>(UD);
      if (!TAD) return;
      auto *TemplateParams = TAD->getTemplateParameters();
      if (!TemplateParams) return;
      auto UnderlyingType = UD->getUnderlyingType();
      if (!UnderlyingType->isTemplateType()) return;
      auto *TemplateType = UnderlyingType->getAsTemplateRecord();
      if (!TemplateType) return;
      auto TemplateArgs = UnderlyingType->template_arguments();
      if (TemplateArgs.size() < 2) return;
      //(optional)Record the node information to be used in the mutation process
      SourceRange SR = UD->getSourceRange();
      string OriginalText = stringutils::rangetoStr(*Result.SourceManager, SR);
      //Perform mutation on the source code text by applying string replacement
      //Check if alias already has variadic template parameter
      bool HasVariadic = false;
      for (unsigned i = 0; i < TemplateParams->size(); ++i) {
        auto *Param = TemplateParams->getParam(i);
        if (Param->isTemplateParameterPack()) {
          HasVariadic = true;
          break;
        }
      }
      string MutatedText;
      if (HasVariadic) {
        //Use existing pack
        MutatedText = OriginalText;
        //Find a pack parameter name
        string PackName;
        for (unsigned i = 0; i < TemplateParams->size(); ++i) {
          auto *Param = TemplateParams->getParam(i);
          if (Param->isTemplateParameterPack()) {
            PackName = Param->getNameAsString();
            break;
          }
        }
        if (PackName.empty()) return;
        //Replace a selected argument with PackName...
        //Select argument index (not first)
        unsigned ArgIdx = getrandom::getRandomIndex(TemplateArgs.size() - 2) + 1;
        //Need to locate the argument in source text
        //Simplified approach: replace the whole alias definition
        string AliasName = UD->getNameAsString();
        string TemplateParamsText = stringutils::rangetoStr(*Result.SourceManager, TemplateParams->getSourceRange());
        string UnderlyingTypeText = stringutils::rangetoStr(*Result.SourceManager, UnderlyingType->getSourceRange());
        //Construct new underlying type with pack expansion
        //We'll replace the whole using declaration
        MutatedText = "template " + TemplateParamsText + " using " + AliasName + " = ";
        //Rebuild underlying type with pack expansion at ArgIdx
        string NewUnderlying;
        auto *TemplateDecl = TemplateType->getTemplateDecl();
        string TemplateName = TemplateDecl->getNameAsString();
        NewUnderlying = TemplateName + "<";
        for (unsigned i = 0; i < TemplateArgs.size(); ++i) {
          if (i == ArgIdx) {
            NewUnderlying += PackName + "...";
          } else {
            NewUnderlying += stringutils::rangetoStr(*Result.SourceManager, TemplateArgs[i]->getSourceRange());
          }
          if (i != TemplateArgs.size() - 1) NewUnderlying += ", ";
        }
        NewUnderlying += ">";
        MutatedText += NewUnderlying;
      } else {
        //Introduce new template parameter pack
        string AliasName = UD->getNameAsString();
        string TemplateParamsText = stringutils::rangetoStr(*Result.SourceManager, TemplateParams->getSourceRange());
        //Add typename... Pack to template parameters
        string NewTemplateParams = TemplateParamsText;
        if (NewTemplateParams.back() == '>') {
          NewTemplateParams.insert(NewTemplateParams.length() - 1, ", typename... Pack");
        } else {
          //Handle case where parameters are not simple
          //Insert before closing >
          size_t Pos = NewTemplateParams.find_last_of('>');
          if (Pos != string::npos) {
            NewTemplateParams.insert(Pos - 1, ", typename... Pack");
          }
        }
        string UnderlyingTypeText = stringutils::rangetoStr(*Result.SourceManager, UnderlyingType->getSourceRange());
        //Select argument index (not first)
        unsigned ArgIdx = getrandom::getRandomIndex(TemplateArgs.size() - 2) + 1;
        //Construct new underlying type with Pack...
        string NewUnderlying;
        auto *TemplateDecl = TemplateType->getTemplateDecl();
        string TemplateName = TemplateDecl->getNameAsString();
        NewUnderlying = TemplateName + "<";
        for (unsigned i = 0; i < TemplateArgs.size(); ++i) {
          if (i == ArgIdx) {
            NewUnderlying += "Pack...";
          } else {
            NewUnderlying += stringutils::rangetoStr(*Result.SourceManager, TemplateArgs[i]->getSourceRange());
          }
          if (i != TemplateArgs.size() - 1) NewUnderlying += ", ";
        }
        NewUnderlying += ">";
        MutatedText = "template " + NewTemplateParams + " using " + AliasName + " = " + NewUnderlying;
      }
      MutatedText = "/*mut225*/" + MutatedText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(SR), MutatedText);
    }
}
  
void MutatorFrontendAction_225::MutatorASTConsumer_225::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typeAliasDecl().bind("UsingDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}