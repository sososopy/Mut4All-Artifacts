//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Explicit_Instantiation_Declaration_455
 */ 
class MutatorFrontendAction_455 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(455)

private:
    class MutatorASTConsumer_455 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_455(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl*> constrainedFunctions;
        std::vector<const clang::ClassTemplateDecl*> classTemplates;
    };
};

//source file
#include "../include/Mutator_Replace_Explicit_Instantiation_Declaration_455.h"

// ========================================================================================================
#define MUT455_OUTPUT 1

void MutatorFrontendAction_455::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("constrainedFunction")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Record constrained member functions
      if (FD->isTemplateInstantiation() && FD->hasAttr<clang::ConceptAttr>()) {
        constrainedFunctions.push_back(FD);
      }
    }
    else if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("classTemplate")) {
      //Filter nodes in header files
      if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTD->getLocation()))
        return;
      //Record class templates
      classTemplates.push_back(CTD);
    }
    else if (auto *EID = Result.Nodes.getNodeAs<clang::FunctionDecl>("explicitInstantiation")) {
      //Filter nodes in header files
      if (!EID || !Result.Context->getSourceManager().isWrittenInMainFile(
                     EID->getLocation()))
        return;
      //Get the source code text of target node
      if (EID->getTemplateSpecializationInfo() && EID->getTemplateSpecializationInfo()->isExplicitSpecialization()) {
        //Find matching constrained function
        for (auto* CF : constrainedFunctions) {
          if (CF->getNameAsString() == EID->getNameAsString()) {
            //Check if template arguments satisfy constraint
            auto* TSI = CF->getTemplateSpecializationInfo();
            if (TSI) {
              auto* TArgs = TSI->getTemplate();
              //Evaluate constraint (simplified check)
              bool constraintSatisfied = true; // Assume satisfied for mutation
              if (constraintSatisfied) {
                //Perform mutation on the source code text by applying string replacement
                std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), EID->getSourceRange());
                std::string requiresClause = " requires ";
                //Extract constraint text from constrained function
                auto* ConceptAttr = CF->getAttr<clang::ConceptAttr>();
                if (ConceptAttr) {
                  std::string constraintText = ConceptAttr->getConstraintExpr()->getSourceRange().getAsString();
                  requiresClause += constraintText;
                }
                //Insert requires clause before semicolon
                size_t semicolonPos = originalText.find(';');
                if (semicolonPos != std::string::npos) {
                  std::string mutatedText = originalText.substr(0, semicolonPos) + requiresClause + originalText.substr(semicolonPos);
                  //Replace the original AST node with the mutated one
                  Rewrite.ReplaceText(CharSourceRange::getTokenRange(EID->getSourceRange()), mutatedText);
                }
              }
            }
          }
        }
      }
    }
    else if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("addInstantiation")) {
      //Filter nodes in header files
      if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTD->getLocation()))
        return;
      //Add explicit instantiation if none exists
      //Find constrained member function in this template
      for (auto* CF : constrainedFunctions) {
        if (CF->getDeclContext() == CTD->getDeclContext()) {
          //Check if template arguments satisfy constraint
          bool constraintSatisfied = true; // Assume satisfied
          if (constraintSatisfied) {
            //Perform mutation on the source code text by applying string replacement
            std::string templateName = CTD->getNameAsString();
            std::string functionName = CF->getNameAsString();
            std::string requiresClause = " requires ";
            auto* ConceptAttr = CF->getAttr<clang::ConceptAttr>();
            if (ConceptAttr) {
              std::string constraintText = ConceptAttr->getConstraintExpr()->getSourceRange().getAsString();
              requiresClause += constraintText;
            }
            std::string instantiationText = "template void " + templateName + "<int>::" + functionName + "()" + requiresClause + ";";
            //Insert after class template definition
            SourceLocation insertLoc = CTD->getEndLoc();
            Rewrite.InsertTextAfterToken(insertLoc, "\n/*mut455*/" + instantiationText);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_455::MutatorASTConsumer_455::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher constrainedMatcher = functionDecl(hasAttr(clang::attr::Concept)).bind("constrainedFunction");
    DeclarationMatcher templateMatcher = classTemplateDecl().bind("classTemplate");
    DeclarationMatcher instantiationMatcher = functionDecl(isExplicitSpecialization()).bind("explicitInstantiation");
    DeclarationMatcher addInstantiationMatcher = classTemplateDecl().bind("addInstantiation");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(constrainedMatcher, &callback);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(instantiationMatcher, &callback);
    matchFinder.addMatcher(addInstantiationMatcher, &callback);
    matchFinder.matchAST(Context);
}