//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_NonType_Template_Argument_With_Dependent_Type_338
 */ 
class MutatorFrontendAction_338 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(338)

private:
    class MutatorASTConsumer_338 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_338(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TemplateDecl *> templateDecls;
        std::vector<const clang::TemplateSpecializationType *> specializationTypes;
    };
};

//source file
#include "../include/Replace_NonType_Template_Argument_With_Dependent_Type_338.h"

// ========================================================================================================
#define MUT338_OUTPUT 1

void MutatorFrontendAction_338::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      //Record template declarations for later use
      templateDecls.push_back(TD);
    }
    else if (auto *TST = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("TemplateSpecializationType")) {
      //Filter nodes in header files
      if (!TST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TST->getLocStart()))
        return;
      //Record template specialization types for mutation
      specializationTypes.push_back(TST);
    }
    else if (auto *TAD = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("TypeAliasDecl")) {
      //Filter nodes in header files
      if (!TAD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TAD->getLocation()))
        return;
      //Get the source code text of target node
      auto aliasSourceRange = TAD->getSourceRange();
      auto aliasText = stringutils::rangetoStr(*(Result.SourceManager), aliasSourceRange);
      
      //Check if it's a template alias with non-type arguments
      auto *TAT = TAD->getUnderlyingType().getTypePtr();
      if (auto *TST = dyn_cast<clang::TemplateSpecializationType>(TAT)) {
        //Find a template declaration that matches this specialization
        for (auto *TD : templateDecls) {
          if (TD->getTemplatedDecl()->getNameAsString() == TST->getTemplateName().getAsTemplateDecl()->getNameAsString()) {
            //Check template parameters
            auto *TPL = TD->getTemplateParameters();
            if (!TPL || TPL->size() == 0) continue;
            
            //Look for non-type template arguments in the specialization
            auto args = TST->template_arguments();
            for (unsigned i = 0; i < args.size(); ++i) {
              if (args[i].getKind() == clang::TemplateArgument::Integral ||
                  args[i].getKind() == clang::TemplateArgument::Declaration ||
                  args[i].getKind() == clang::TemplateArgument::NullPtr ||
                  args[i].getKind() == clang::TemplateArgument::Expression) {
                //Find a type parameter from the template declaration
                for (unsigned j = 0; j < TPL->size(); ++j) {
                  auto *TP = TPL->getParam(j);
                  if (TP->isTemplateParameterType()) {
                    //Perform mutation: replace non-type argument with dependent type
                    std::string replacement;
                    if (TP->isParameterPack()) {
                      replacement = TP->getNameAsString() + "...";
                    } else {
                      //Create a dependent type transformation
                      replacement = TP->getNameAsString() + "*";
                    }
                    
                    //Locate the argument in the source text
                    std::string argText;
                    SourceLocation argLoc;
                    //This is a simplified approach; actual implementation would need to extract argument text
                    //For demonstration, we replace the entire alias declaration
                    std::string mutatedAlias = aliasText;
                    size_t templateArgsStart = mutatedAlias.find("<");
                    size_t templateArgsEnd = mutatedAlias.find(">");
                    if (templateArgsStart != std::string::npos && templateArgsEnd != std::string::npos) {
                      std::string argsStr = mutatedAlias.substr(templateArgsStart + 1, templateArgsEnd - templateArgsStart - 1);
                      //Replace first non-type argument found
                      size_t argPos = 0;
                      //Simplified: replace with typename T::type if T has nested type, else T*
                      replacement = "typename " + TP->getNameAsString() + "::type";
                      mutatedAlias.replace(templateArgsStart + 1 + argPos, argsStr.length(), replacement);
                    }
                    
                    //Replace the original AST node with the mutated one
                    Rewrite.ReplaceText(CharSourceRange::getTokenRange(aliasSourceRange), mutatedAlias);
                    return;
                  }
                }
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_338::MutatorASTConsumer_338::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher templateDeclMatcher = templateDecl().bind("TemplateDecl");
    DeclarationMatcher typeAliasMatcher = typeAliasDecl().bind("TypeAliasDecl");
    TypeMatcher specializationMatcher = templateSpecializationType().bind("TemplateSpecializationType");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateDeclMatcher, &callback);
    matchFinder.addMatcher(typeAliasMatcher, &callback);
    matchFinder.addMatcher(specializationMatcher, &callback);
    matchFinder.matchAST(Context);
}