//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Alias_Instantiation_130
 */ 
class MutatorFrontendAction_130 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(130)

private:
    class MutatorASTConsumer_130 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_130(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Replace_Template_Alias_Instantiation_130.h"

// ========================================================================================================
#define MUT130_OUTPUT 1

void MutatorFrontendAction_130::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ATD = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("aliasTemplateDecl")) {
      //Filter nodes in header files
      if (!ATD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ATD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      aliasTemplates.push_back(ATD);
    }
    else if (auto *CXXD = Result.Nodes.getNodeAs<clang::CXXDeductionGuideDecl>("deductionGuideDecl")) {
      //Filter nodes in header files
      if (!CXXD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CXXD->getLocation()))
        return;
      //Get the source code text of target node
      auto *TAD = CXXD->getDeducedTemplate();
      if (!TAD) return;
      auto *TD = dyn_cast<clang::TemplateDecl>(TAD);
      if (!TD) return;
      //Check if this template is an alias template
      const clang::TypeAliasTemplateDecl* targetAlias = nullptr;
      for (auto *alias : aliasTemplates) {
        if (alias->getTemplatedDecl() == TD) {
          targetAlias = alias;
          break;
        }
      }
      if (!targetAlias) return;
      //Get the underlying template class
      auto *aliasDecl = targetAlias->getTemplatedDecl();
      auto underlyingType = dyn_cast<clang::TypeAliasDecl>(aliasDecl)->getUnderlyingType();
      auto recordType = underlyingType->getAs<clang::RecordType>();
      if (!recordType) return;
      auto *recordDecl = recordType->getDecl();
      if (!recordDecl) return;
      //Get template arguments from alias
      auto *aliasTemplateParams = targetAlias->getTemplateParameters();
      auto *underlyingTemplate = dyn_cast<clang::ClassTemplateSpecializationDecl>(recordDecl);
      if (!underlyingTemplate) return;
      auto templateArgs = underlyingTemplate->getTemplateArgs();
      //Get deduced template arguments from deduction guide
      auto deducedArgs = CXXD->getDeducedTemplate()->getTemplateArgs().asArray();
      //Build replacement string
      std::string replacement = recordDecl->getNameAsString();
      replacement += "<";
      for (unsigned i = 0; i < templateArgs.size(); ++i) {
        if (i > 0) replacement += ", ";
        if (templateArgs[i].getKind() == clang::TemplateArgument::Type) {
          replacement += templateArgs[i].getAsType().getAsString();
        } else if (templateArgs[i].getKind() == clang::TemplateArgument::Integral) {
          replacement += std::to_string(templateArgs[i].getAsIntegral().getExtValue());
        } else if (templateArgs[i].getKind() == clang::TemplateArgument::Expression) {
          auto expr = templateArgs[i].getAsExpr();
          replacement += stringutils::rangetoStr(*(Result.SourceManager), expr->getSourceRange());
        }
      }
      replacement += ">";
      //Perform mutation on the source code text by applying string replacement
      auto originalText = stringutils::rangetoStr(*(Result.SourceManager), CXXD->getSourceRange());
      size_t aliasPos = originalText.find(TAD->getNameAsString());
      if (aliasPos != std::string::npos) {
        std::string mutatedText = originalText;
        mutatedText.replace(aliasPos, TAD->getNameAsString().length(), replacement);
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(CXXD->getSourceRange()), mutatedText);
      }
    }
}
  
void MutatorFrontendAction_130::MutatorASTConsumer_130::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher aliasMatcher = typeAliasTemplateDecl().bind("aliasTemplateDecl");
    DeclarationMatcher deductionMatcher = cxxDeductionGuideDecl().bind("deductionGuideDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(aliasMatcher, &callback);
    matchFinder.addMatcher(deductionMatcher, &callback);
    matchFinder.matchAST(Context);
}