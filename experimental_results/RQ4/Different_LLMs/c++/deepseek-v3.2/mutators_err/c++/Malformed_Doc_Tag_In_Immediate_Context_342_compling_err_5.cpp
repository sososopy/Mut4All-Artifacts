//header file
#pragma once
#include "Mutator_base.h"

/**
 * Malformed_Doc_Tag_In_Immediate_Context_342
 */ 
class MutatorFrontendAction_342 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(342)

private:
    class MutatorASTConsumer_342 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_342(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> malformed_tags = {"@doc.self", "@param", "@return", "@tparam"};
    };
};

//source file
#include "../include/Malformed_Doc_Tag_In_Immediate_Context_342.h"

// ========================================================================================================
#define MUT342_OUTPUT 1

void MutatorFrontendAction_342::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      
      // Check if function has immediate context (consteval, constexpr, or literal operator)
      bool isImmediateContext = FD->isConsteval() || FD->isConstexpr() || FD->getDeclName().getNameKind() == clang::DeclarationName::CXXLiteralOperatorName;
      
      // Get associated comments
      auto &SM = *Result.SourceManager;
      std::string comment;
      SourceRange commentRange;
      
      // Look for documentation comment before the function
      SourceLocation loc = FD->getBeginLoc();
      if (loc.isValid()) {
        // Search for comment tokens before the function
        clang::comments::FullComment *FC = Result.Context->getCommentForDecl(FD, nullptr);
        if (FC) {
          commentRange = FC->getSourceRange();
          comment = stringutils::rangetoStr(SM, commentRange);
        }
      }
      
      if (comment.empty()) {
        // If no documentation comment found, skip
        return;
      }
      
      //Get the source code text of target node
      //(optional)Record the node information to be used in the mutation process
      //Perform mutation on the source code text by applying string replacement
      // Choose a malformed tag
      std::string tag = malformed_tags[getrandom::getRandomIndex(malformed_tags.size() - 1)];
      
      // Insert tag at beginning of comment
      std::string mutated_comment;
      if (comment.substr(0, 3) == "///") {
        mutated_comment = "/// " + tag + " " + comment.substr(3);
      } else if (comment.substr(0, 3) == "/**") {
        mutated_comment = "/** " + tag + " " + comment.substr(3);
      } else {
        // Other comment formats, just prepend tag
        mutated_comment = tag + " " + comment;
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(commentRange), mutated_comment);
    }
}
  
void MutatorFrontendAction_342::MutatorASTConsumer_342::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(anyOf(isConsteval(), isConstexpr(), hasAttr(clang::attr::CXX11NoReturn))).bind("FuncDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}