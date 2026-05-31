//header file
#pragma once
#include "Mutator_base.h"

/**
 * Malformed_Attribute_Insertion_368
 */ 
class MutatorFrontendAction_368 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(368)

private:
    class MutatorASTConsumer_368 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_368(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::Decl *> declarations;
    };
};

//source file
#include "../include/Malformed_Attribute_Insertion_368.h"

// ========================================================================================================
#define MUT368_OUTPUT 1

void MutatorFrontendAction_368::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *AT = Result.Nodes.getNodeAs<clang::Attr>("Attr")) {
      //Filter nodes in header files
      if (!AT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AT->getLocation()))
        return;
      //Get the source code text of target node
      auto attrText = stringutils::rangetoStr(*(Result.SourceManager),
                                              AT->getRange());
      std::string mutatedAttr;
      //Perform mutation on the source code text by applying string replacement
      if (attrText.find("[[") != std::string::npos) {
        // Standard C++11 attribute
        size_t pos = attrText.find("[[");
        mutatedAttr = attrText.substr(0, pos + 2) + "[[]" + attrText.substr(pos + 2);
      } else if (attrText.find("__attribute__") != std::string::npos) {
        // GCC-style attribute
        size_t pos = attrText.find("((");
        if (pos != std::string::npos) {
          mutatedAttr = attrText.substr(0, pos + 2) + "(()" + attrText.substr(pos + 2);
        } else {
          mutatedAttr = attrText;
        }
      } else if (attrText.find("alignas") != std::string::npos) {
        // alignas attribute
        size_t pos = attrText.find("(");
        if (pos != std::string::npos) {
          mutatedAttr = attrText.substr(0, pos + 1) + "()" + attrText.substr(pos + 1);
        } else {
          mutatedAttr = attrText;
        }
      } else {
        // Unknown attribute type, skip
        return;
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(AT->getRange()), mutatedAttr);
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      declarations.push_back(FD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      declarations.push_back(VD);
    }
}

void MutatorFrontendAction_368::MutatorASTConsumer_368::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher attrMatcher = attr().bind("Attr");
    DeclarationMatcher funcMatcher = functionDecl().bind("FuncDecl");
    DeclarationMatcher varMatcher = varDecl().bind("VarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(attrMatcher, &callback);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.matchAST(Context);
    // If no attributes were found, insert a malformed attribute on a random declaration
    if (callback.declarations.empty()) {
        return;
    }
    size_t idx = getrandom::getRandomIndex(callback.declarations.size() - 1);
    const clang::Decl *decl = callback.declarations[idx];
    std::string malformedAttr = "[[[]]] ";
    SourceLocation loc = decl->getBeginLoc();
    callback.Rewrite.InsertTextBefore(loc, malformedAttr);
}