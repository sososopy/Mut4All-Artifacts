//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Default_Template_Argument_In_Alias_Template_With_Dependent_Type_407
 */ 
class MutatorFrontendAction_407 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(407)

private:
    class MutatorASTConsumer_407 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_407(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::NamedDecl*> availableTemplates;
        std::vector<const clang::TemplateTypeParmDecl*> availableTemplateParams;
    };
};

//source file
#include "../include/Mutator_Replace_Default_Template_Argument_In_Alias_Template_With_Dependent_Type_407.h"

// ========================================================================================================
#define MUT407_OUTPUT 1

void MutatorFrontendAction_407::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("AliasTemplate")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto aliasTemplateDecl = MT;
      auto templateParams = aliasTemplateDecl->getTemplateParameters();
      if (templateParams->size() == 0) return;
      auto firstParam = dyn_cast<TemplateTypeParmDecl>(*templateParams->begin());
      if (!firstParam || !firstParam->hasDefaultArgument()) return;
      
      //Record available templates and template parameters from the seed
      if (auto *TD = Result.Nodes.getNodeAs<clang::NamedDecl>("AvailableTemplate")) {
        if (TD && Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation())) {
          availableTemplates.push_back(TD);
        }
      }
      if (auto *TP = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("AvailableTemplateParam")) {
        if (TP && Result.Context->getSourceManager().isWrittenInMainFile(TP->getLocation())) {
          availableTemplateParams.push_back(TP);
        }
      }
      
      //Perform mutation on the source code text by applying string replacement
      std::string newDefaultArg;
      if (availableTemplates.empty()) {
        //Introduce dummy template
        newDefaultArg = "Dummy<T>";
        SourceLocation insertionLoc = aliasTemplateDecl->getBeginLoc();
        Rewrite.InsertTextBefore(insertionLoc, "template<typename> struct Dummy{};\n");
      } else {
        //Use existing template
        size_t index = getrandom::getRandomIndex(availableTemplates.size() - 1);
        auto chosenTemplate = availableTemplates[index];
        std::string templateName = chosenTemplate->getNameAsString();
        
        //Find a suitable template parameter from enclosing scope
        std::string paramName;
        if (availableTemplateParams.empty()) {
          paramName = "T";
        } else {
          size_t paramIndex = getrandom::getRandomIndex(availableTemplateParams.size() - 1);
          paramName = availableTemplateParams[paramIndex]->getNameAsString();
        }
        
        newDefaultArg = templateName + "<" + paramName + ">";
      }
      
      //Replace the default argument in the alias template
      auto defaultArgRange = firstParam->getDefaultArgInfo()->getSourceRange();
      Rewrite.ReplaceText(defaultArgRange, newDefaultArg);
    }
}
  
void MutatorFrontendAction_407::MutatorASTConsumer_407::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher aliasMatcher = typeAliasTemplateDecl().bind("AliasTemplate");
    DeclarationMatcher templateMatcher = namedDecl(anyOf(templateDecl(), classTemplateDecl(), recordDecl(isStruct(), isDefinition()))).bind("AvailableTemplate");
    DeclarationMatcher paramMatcher = templateTypeParmDecl().bind("AvailableTemplateParam");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(aliasMatcher, &callback);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(paramMatcher, &callback);
    matchFinder.matchAST(Context);
}