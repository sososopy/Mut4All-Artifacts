//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_String_Literal_Template_Argument_With_Char_Array_98
 */ 
class MutatorFrontendAction_98 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(98)

private:
    class MutatorASTConsumer_98 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_98(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> insertedArrays;
    };
};

//source file
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT98_OUTPUT 1

void MutatorFrontendAction_98::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TL = Result.Nodes.getNodeAs<clang::StringLiteral>("StringLiteral")) {
      //Filter nodes in header files
      if (!TL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TL->getLocation()))
        return;
      
      // Get the parent template argument
      auto *Parent = Result.Nodes.getNodeAs<clang::NonTypeTemplateParmDecl>("TemplateParam");
      if (!Parent) return;
      
      // Check if we're in a template argument list
      auto *TAL = Result.Nodes.getNodeAs<clang::TemplateArgumentLoc>("TemplateArg");
      if (!TAL) return;
      
      // Get the string literal value and size
      std::string literal = TL->getString().str();
      unsigned length = TL->getLength();
      unsigned arraySize = length + 1; // +1 for null terminator
      
      // Generate unique array name
      std::string arrayName = "mut98_arr_" + std::to_string(insertedArrays.size());
      while (insertedArrays.find(arrayName) != insertedArrays.end()) {
        arrayName = "mut98_arr_" + std::to_string(insertedArrays.size() + rand());
      }
      
      // Create array declaration
      std::string arrayDecl = "const char " + arrayName + "[" + std::to_string(arraySize) + "] = \"" + literal + "\";\n";
      
      // Get the location where to insert (before the template instantiation)
      auto *DRE = Result.Nodes.getNodeAs<clang::DeclRefExpr>("TemplateRef");
      if (!DRE) return;
      
      SourceLocation insertLoc = DRE->getBeginLoc();
      
      // Insert array declaration before the template instantiation
      Rewrite.InsertText(insertLoc, "/*mut98*/" + arrayDecl, true, true);
      
      // Replace the string literal with array name in template argument
      SourceRange literalRange = TL->getSourceRange();
      Rewrite.ReplaceText(literalRange, arrayName);
      
      insertedArrays.insert(arrayName);
    }
}
  
void MutatorFrontendAction_98::MutatorASTConsumer_98::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = stringLiteral(hasParent(templateArgument()),
                                               hasAncestor(nonTypeTemplateParmDecl().bind("TemplateParam")),
                                               hasAncestor(templateArgumentLoc().bind("TemplateArg")),
                                               hasAncestor(declRefExpr().bind("TemplateRef"))).bind("StringLiteral");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}