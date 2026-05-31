//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Default_Template_Argument_In_Alias_Template_149
 */ 
class MutatorFrontendAction_149 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(149)

private:
    class MutatorASTConsumer_149 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_149(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Default_Template_Argument_In_Alias_Template_149.h"

// ========================================================================================================
#define MUT149_OUTPUT 1

void MutatorFrontendAction_149::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *AT = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("aliasTemplate")) {
      //Filter nodes in header files
      if (!AT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AT->getLocation()))
        return;
      
      auto *TAD = AT->getTemplatedDecl();
      if (!TAD) return;
      
      // Get the alias template's template parameters
      auto *TP = AT->getTemplateParameters();
      if (!TP || TP->size() == 0) return;
      
      // Check if any template parameter has a default argument that is a simple type (non-dependent)
      bool hasSimpleDefault = false;
      unsigned paramIndex = 0;
      for (unsigned i = 0; i < TP->size(); ++i) {
        auto *P = TP->getParam(i);
        if (auto *TTP = dyn_cast<TemplateTypeParmDecl>(P)) {
          if (TTP->hasDefaultArgument() && !TTP->getDefaultArgument()->isDependentType()) {
            hasSimpleDefault = true;
            paramIndex = i;
            break;
          }
        }
      }
      
      if (!hasSimpleDefault) return;
      
      // Find the enclosing template (class or function template)
      auto *EnclosingDC = dyn_cast_or_null<DeclContext>(AT->getDeclContext());
      while (EnclosingDC && !(isa<ClassTemplateDecl>(EnclosingDC) || isa<FunctionTemplateDecl>(EnclosingDC))) {
        EnclosingDC = dyn_cast_or_null<DeclContext>(EnclosingDC->getParent());
      }
      
      if (!EnclosingDC) return;
      
      // Get the enclosing template's parameters
      TemplateParameterList *EnclosingTP = nullptr;
      if (auto *CTD = dyn_cast<ClassTemplateDecl>(EnclosingDC)) {
        EnclosingTP = CTD->getTemplateParameters();
      } else if (auto *FTD = dyn_cast<FunctionTemplateDecl>(EnclosingDC)) {
        EnclosingTP = FTD->getTemplateParameters();
      }
      
      if (!EnclosingTP || EnclosingTP->size() == 0) return;
      
      // Find the first type parameter in the enclosing template
      std::string outerTypeParamName;
      for (unsigned i = 0; i < EnclosingTP->size(); ++i) {
        auto *P = EnclosingTP->getParam(i);
        if (auto *TTP = dyn_cast<TemplateTypeParmDecl>(P)) {
          outerTypeParamName = TTP->getNameAsString();
          break;
        }
      }
      
      if (outerTypeParamName.empty()) return;
      
      //Get the source code text of target node
      auto aliasDeclStr = stringutils::rangetoStr(*(Result.SourceManager),
                                                 AT->getSourceRange());
      
      // Find the template parameter with default argument in the source text
      auto *Param = TP->getParam(paramIndex);
      auto paramRange = Param->getSourceRange();
      auto paramStr = stringutils::rangetoStr(*(Result.SourceManager), paramRange);
      
      // Replace the default argument with the outer template parameter
      size_t eqPos = paramStr.find('=');
      if (eqPos == std::string::npos) return;
      
      std::string newParamStr = paramStr.substr(0, eqPos + 2) + outerTypeParamName;
      
      //Perform mutation on the source code text by applying string replacement
      aliasDeclStr.replace(paramRange.getBegin().getRawEncoding() - AT->getBeginLoc().getRawEncoding(),
                          paramStr.length(),
                          newParamStr);
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(AT->getSourceRange()), aliasDeclStr);
    }
}
  
void MutatorFrontendAction_149::MutatorASTConsumer_149::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typeAliasTemplateDecl().bind("aliasTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}