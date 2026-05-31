//header file
#pragma once
#include "Mutator_base.h"

/**
 * Corrupt_Make_Integer_Seq_Template_178
 */ 
class MutatorFrontendAction_178 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(178)

private:
    class MutatorASTConsumer_178 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_178(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> addedTemplates;
    };
};

//source file
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT178_OUTPUT 1

void MutatorFrontendAction_178::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DS = Result.Nodes.getNodeAs<clang::DeclRefExpr>("makeIntegerSeq")) {
      //Filter nodes in header files
      if (!DS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DS->getLocation()))
        return;
      
      // Check if this is actually __make_integer_seq
      if (DS->getNameInfo().getAsString() != "__make_integer_seq")
        return;
        
      // Get the parent call expression to access template arguments
      auto* CE = llvm::dyn_cast<clang::CallExpr>(DS->getParent());
      if (!CE) return;
      
      // Get the callee to access template arguments
      auto* CEDecl = CE->getCalleeDecl();
      if (!CEDecl) return;
      
      // Try to get the function declaration
      auto* FD = llvm::dyn_cast<clang::FunctionDecl>(CEDecl);
      if (!FD) return;
      
      // Get template arguments
      auto* TSI = FD->getTemplateSpecializationInfo();
      if (!TSI) return;
      
      const clang::TemplateArgumentList* TAL = TSI->getTemplateArgs();
      if (!TAL || TAL->size() < 3) return;
      
      // First template argument should be a template
      const clang::TemplateArgument& firstArg = TAL->get(0);
      if (firstArg.getKind() != clang::TemplateArgument::Template)
        return;
        
      clang::TemplateName TN = firstArg.getAsTemplate();
      clang::TemplateDecl* TD = TN.getAsTemplateDecl();
      if (!TD) return;
      
      // Check if it's a class template
      auto* CTD = llvm::dyn_cast<clang::ClassTemplateDecl>(TD);
      if (!CTD) return;
      
      // Get source location for insertion
      SourceManager& SM = Rewrite.getSourceMgr();
      SourceLocation InsertLoc = SM.getLocForEndOfFile(SM.getMainFileID());
      
      // Generate mutated template name
      std::string mutatedTemplateName = "MutatedTemplate178";
      
      // Check if we already added this template
      if (addedTemplates.find(mutatedTemplateName) == addedTemplates.end()) {
        // Add template declaration with insufficient parameters
        std::string templateDecl = "\ntemplate <class>\nstruct " + mutatedTemplateName + ";\n";
        Rewrite.InsertTextBefore(InsertLoc, templateDecl);
        addedTemplates.insert(mutatedTemplateName);
      }
      
      // Get the source range of the template name in __make_integer_seq
      SourceRange NameRange = DS->getSourceRange();
      std::string OriginalText = stringutils::rangetoStr(SM, NameRange);
      
      // Find the position of the first template argument
      std::size_t templateStart = OriginalText.find('<');
      if (templateStart == std::string::npos) return;
      
      std::size_t firstComma = OriginalText.find(',', templateStart);
      if (firstComma == std::string::npos) return;
      
      // Replace the first template argument
      std::string MutatedText = OriginalText.substr(0, templateStart + 1);
      MutatedText += mutatedTemplateName;
      MutatedText += OriginalText.substr(firstComma);
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(NameRange, MutatedText);
    }
}
  
void MutatorFrontendAction_178::MutatorASTConsumer_178::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = declRefExpr(hasName("__make_integer_seq")).bind("makeIntegerSeq");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}