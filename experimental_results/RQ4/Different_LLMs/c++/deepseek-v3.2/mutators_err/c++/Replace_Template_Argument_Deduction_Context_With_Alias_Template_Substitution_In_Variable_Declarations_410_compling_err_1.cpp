//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Argument_Deduction_Context_With_Alias_Template_Substitution_In_Variable_Declarations_410
 */ 
class MutatorFrontendAction_410 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(410)

private:
    class MutatorASTConsumer_410 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_410(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TypeAliasTemplateDecl*> aliasTemplates;
    };
};

//source file
#include "../include/Replace_Template_Argument_Deduction_Context_With_Alias_Template_Substitution_In_Variable_Declarations_410.h"

// ========================================================================================================
#define MUT410_OUTPUT 1

void MutatorFrontendAction_410::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Check if variable uses CTAD (type deduced)
      if (!VD->getType()->isDependentType() && VD->getTypeSourceInfo()) {
        auto TSI = VD->getTypeSourceInfo();
        auto TL = TSI->getTypeLoc();
        if (TL.getTypeLocClass() == clang::TypeLoc::DeducedTemplateSpecialization) {
          //Get the deduced type
          auto DeducedType = VD->getType();
          //Find if deduced type is an alias template specialization
          for (auto AliasDecl : aliasTemplates) {
            auto AliasType = AliasDecl->getUnderlyingDecl()->getUnderlyingType();
            if (AliasType.getTypePtr() == DeducedType.getTypePtr()) {
              //Get the source code text of target node
              auto VarRange = VD->getSourceRange();
              auto VarText = stringutils::rangetoStr(*(Result.SourceManager), VarRange);
              //Perform mutation on the source code text by applying string replacement
              //Replace variable declaration with alias template with auto placeholders
              auto AliasName = AliasDecl->getNameAsString();
              auto NumParams = AliasDecl->getTemplateParameters()->size();
              std::string AutoPlaceholders;
              for (unsigned i = 0; i < NumParams; ++i) {
                AutoPlaceholders += "auto";
                if (i != NumParams - 1) AutoPlaceholders += ", ";
              }
              //Construct new declaration
              std::string NewDecl = AliasName + "<" + AutoPlaceholders + "> " + VD->getNameAsString();
              //Keep initializer
              auto Init = VD->getInit();
              if (Init) {
                auto InitRange = Init->getSourceRange();
                auto InitText = stringutils::rangetoStr(*(Result.SourceManager), InitRange);
                NewDecl += " = " + InitText;
              }
              NewDecl = "/*mut410*/" + NewDecl;
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(VarRange), NewDecl);
              return;
            }
          }
        }
      }
    }
    //Record alias templates when matched
    else if (auto *ATD = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("AliasTemplate")) {
      if (!ATD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ATD->getLocation()))
        return;
      aliasTemplates.push_back(ATD);
    }
}
  
void MutatorFrontendAction_410::MutatorASTConsumer_410::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher varMatcher = varDecl(hasInitializer()).bind("VarDecl");
    DeclarationMatcher aliasMatcher = typeAliasTemplateDecl().bind("AliasTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(aliasMatcher, &callback);
    matchFinder.matchAST(Context);
}