//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Inline_Variable_Template_38
 */ 
class MutatorFrontendAction_38 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(38)

private:
    class MutatorASTConsumer_38 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_38(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Replace_Inline_Variable_Template_38.h"

// ========================================================================================================
#define MUT38_OUTPUT 1

void MutatorFrontendAction_38::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarTemplateDecl>("varTemplateDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      
      auto *TemplatedDecl = VD->getTemplatedDecl();
      if (!TemplatedDecl)
        return;
      
      // Check if it's an inline variable template
      if (!TemplatedDecl->isInline())
        return;
      
      // Check if it's inside a struct/class/union
      auto *Ctx = dyn_cast<clang::RecordDecl>(TemplatedDecl->getDeclContext());
      if (!Ctx)
        return;
      
      //Get the source code text of target node
      SourceRange FullRange = VD->getSourceRange();
      std::string OriginalText = stringutils::rangetoStr(*(Result.SourceManager), FullRange);
      
      // Check if it's static member
      bool IsStatic = TemplatedDecl->isStaticDataMember();
      
      // Perform mutation based on static/non-static
      std::string MutatedText;
      if (IsStatic) {
        // For static: remove inline from inside class
        size_t InlinePos = OriginalText.find("inline");
        if (InlinePos != std::string::npos) {
          MutatedText = OriginalText;
          MutatedText.erase(InlinePos, 6); // Remove "inline"
          // Remove extra whitespace
          if (MutatedText[InlinePos] == ' ')
            MutatedText.erase(InlinePos, 1);
        }
      } else {
        // For non-static: move declaration outside with explicit specialization
        // First, get variable name and template parameters
        std::string VarName = TemplatedDecl->getNameAsString();
        std::string ClassName = Ctx->getNameAsString();
        
        // Extract template parameters from original declaration
        std::string TemplateParams;
        if (auto *TPL = VD->getTemplateParameters()) {
          SourceRange TPRange = TPL->getSourceRange();
          TemplateParams = stringutils::rangetoStr(*(Result.SourceManager), TPRange);
        }
        
        // Create inside class declaration (without inline and initializer)
        size_t EqPos = OriginalText.find('=');
        std::string InsideDecl;
        if (EqPos != std::string::npos) {
          InsideDecl = OriginalText.substr(0, EqPos);
          // Remove "inline" keyword
          size_t InlinePos = InsideDecl.find("inline");
          if (InlinePos != std::string::npos) {
            InsideDecl.erase(InlinePos, 6);
            if (InsideDecl[InlinePos] == ' ')
              InsideDecl.erase(InlinePos, 1);
          }
          InsideDecl += ";";
        }
        
        // Create outside definition with explicit specialization
        std::string OutsideDef = "template<>\ninline ";
        size_t TypeStart = OriginalText.find_first_not_of(" \t\n", OriginalText.find("inline") + 6);
        std::string TypeAndName = OriginalText.substr(TypeStart);
        OutsideDef += TypeAndName + ";\n";
        
        MutatedText = InsideDecl + "\n" + OutsideDef;
      }
      
      //Replace the original AST node with the mutated one
      if (!MutatedText.empty()) {
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FullRange), MutatedText);
      }
    }
}
  
void MutatorFrontendAction_38::MutatorASTConsumer_38::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varTemplateDecl(isInline(), hasAncestor(recordDecl())).bind("varTemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}