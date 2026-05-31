//header file
#pragma once
#include "Mutator_base.h"

/**
 * Split_Template_Definition_406
 */ 
class MutatorFrontendAction_406 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(406)

private:
    class MutatorASTConsumer_406 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_406(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionTemplateDecl*> templateFuncs;
        std::vector<const clang::ClassTemplateDecl*> templateClasses;
    };
};

//source file
#include "../include/MutatorSplit_Template_Definition_406.h"

// ========================================================================================================
#define MUT406_OUTPUT 1

void MutatorFrontendAction_406::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateFunc")) {
      //Filter nodes in header files
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FTD->getLocation()))
        return;
      //Check if it's in a module interface unit
      if (!FTD->getDeclContext()->isModuleContext())
        return;
      //Check if it's exported
      if (!FTD->hasAttr<clang::ExportAttr>())
        return;
      //Check if it has a definition
      if (!FTD->getTemplatedDecl()->hasBody())
        return;
      //Record the node information to be used in the mutation process
      templateFuncs.push_back(FTD);
    }
    else if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateClass")) {
      //Filter nodes in header files
      if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTD->getLocation()))
        return;
      //Check if it's in a module interface unit
      if (!CTD->getDeclContext()->isModuleContext())
        return;
      //Check if it's exported
      if (!CTD->hasAttr<clang::ExportAttr>())
        return;
      //Check if it has a definition
      if (!CTD->getTemplatedDecl()->isCompleteDefinition())
        return;
      //Record the node information to be used in the mutation process
      templateClasses.push_back(CTD);
    }
    else if (auto *TU = Result.Nodes.getNodeAs<clang::TranslationUnitDecl>("TranslationUnit")) {
      //Perform mutation on the recorded templates
      if (!templateFuncs.empty() || !templateClasses.empty()) {
        //Choose one template to mutate
        int choice = getrandom::getRandomIndex(templateFuncs.size() + templateClasses.size() - 1);
        if (choice < templateFuncs.size()) {
          const clang::FunctionTemplateDecl* FTD = templateFuncs[choice];
          //Get the source code text of target node
          auto sourceRange = FTD->getSourceRange();
          std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
          //Split the template: keep forward declaration in interface, move definition to implementation
          //Find the function body
          const clang::FunctionDecl* FD = FTD->getTemplatedDecl();
          if (FD && FD->hasBody()) {
            auto bodyRange = FD->getBody()->getSourceRange();
            std::string bodyText = stringutils::rangetoStr(*(Result.SourceManager), bodyRange);
            //Create forward declaration without body
            std::string forwardDecl = originalText.substr(0, originalText.find(bodyText));
            forwardDecl += ";";
            //Replace the original AST node with the mutated one (forward declaration)
            Rewrite.ReplaceText(sourceRange, forwardDecl);
            //Create implementation unit content (not exported)
            std::string implText = "module " + FTD->getOwningModule()->Name->getName() + ";\n";
            implText += originalText.substr(0, originalText.find(bodyText));
            implText += bodyText;
            //Insert implementation after the interface (simulating separate file)
            Rewrite.InsertTextAfterToken(sourceRange.getEnd(), "\n/*mut406*/" + implText);
          }
        } else {
          const clang::ClassTemplateDecl* CTD = templateClasses[choice - templateFuncs.size()];
          //Get the source code text of target node
          auto sourceRange = CTD->getSourceRange();
          std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
          //Split the template: keep forward declaration in interface, move definition to implementation
          //Find the class body
          const clang::CXXRecordDecl* RD = CTD->getTemplatedDecl();
          if (RD && RD->isCompleteDefinition()) {
            //Create forward declaration without definition
            std::string forwardDecl = originalText.substr(0, originalText.find("{"));
            forwardDecl += ";";
            //Replace the original AST node with the mutated one (forward declaration)
            Rewrite.ReplaceText(sourceRange, forwardDecl);
            //Create implementation unit content (not exported)
            std::string implText = "module " + CTD->getOwningModule()->Name->getName() + ";\n";
            implText += originalText;
            //Insert implementation after the interface (simulating separate file)
            Rewrite.InsertTextAfterToken(sourceRange.getEnd(), "\n/*mut406*/" + implText);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_406::MutatorASTConsumer_406::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher funcMatcher = functionTemplateDecl(isExported(), hasAncestor(moduleDecl())).bind("TemplateFunc");
    DeclarationMatcher classMatcher = classTemplateDecl(isExported(), hasAncestor(moduleDecl())).bind("TemplateClass");
    DeclarationMatcher tuMatcher = translationUnitDecl().bind("TranslationUnit");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.addMatcher(tuMatcher, &callback);
    matchFinder.matchAST(Context);
}