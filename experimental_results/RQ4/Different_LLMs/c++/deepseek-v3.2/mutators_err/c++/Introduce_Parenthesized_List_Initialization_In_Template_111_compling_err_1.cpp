//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Parenthesized_List_Initialization_In_Template_111
 */ 
class MutatorFrontendAction_111 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(111)

private:
    class MutatorASTConsumer_111 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_111(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Introduce_Parenthesized_List_Initialization_In_Template_111.h"

// ========================================================================================================
#define MUT111_OUTPUT 1

void MutatorFrontendAction_111::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      
      // Ensure we are inside a template context
      if (!VD->getDeclContext()->isDependentContext() && 
          !isa<FunctionDecl>(VD->getDeclContext())->isTemplated())
        return;
      
      // Get the initializer
      Expr *Init = VD->getInit();
      if (!Init) return;
      
      // Check if it's a direct initialization (already parenthesized) or copy initialization
      // We target copy initialization (T var = expr) or brace initialization (T var{expr})
      bool isCopyInit = VD->getInitStyle() == VarDecl::InitializationStyle::CInit;
      bool isBraceInit = VD->getInitStyle() == VarDecl::InitializationStyle::ListInit;
      
      if (!isCopyInit && !isBraceInit) return;
      
      // Get source ranges
      SourceRange VarRange = VD->getSourceRange();
      SourceLocation InitStart = Init->getBeginLoc();
      SourceLocation InitEnd = Init->getEndLoc();
      
      // Get the variable name and type
      std::string VarName = VD->getNameAsString();
      if (VarName.empty()) return;
      
      // Get the initializer expression text
      std::string InitText = stringutils::rangetoStr(*(Result.SourceManager), 
                                                     SourceRange(InitStart, InitEnd));
      
      // Construct parenthesized initialization
      std::string NewInit = "(" + InitText + ")";
      
      // Determine the replacement range
      // For copy initialization: replace "= expr" with "(expr)"
      // For brace initialization: replace "{expr}" with "(expr)"
      SourceLocation ReplaceStart;
      SourceLocation ReplaceEnd;
      
      if (isCopyInit) {
        // Find the '=' location
        ReplaceStart = InitStart.getLocWithOffset(-1); // Start before '='
        ReplaceEnd = InitEnd;
      } else { // isBraceInit
        ReplaceStart = InitStart; // Start at '{'
        ReplaceEnd = InitEnd;
      }
      
      // Perform the replacement
      Rewrite.ReplaceText(SourceRange(ReplaceStart, ReplaceEnd), NewInit);
    }
}
  
void MutatorFrontendAction_111::MutatorASTConsumer_111::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(
        hasAncestor(decl(anyOf(
            functionTemplateDecl(),
            classTemplateDecl()
        ))),
        unless(parmVarDecl())
    ).bind("varDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}