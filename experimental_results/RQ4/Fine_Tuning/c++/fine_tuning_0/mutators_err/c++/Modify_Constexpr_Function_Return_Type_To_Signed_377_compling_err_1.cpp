//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Constexpr_Function_Return_Type_To_Signed_377
 */ 
class MutatorFrontendAction_377 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(377)

private:
    class MutatorASTConsumer_377 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_377(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Constexpr_Function_Return_Type_To_Signed_377.h"

// ========================================================================================================
#define MUT377_OUTPUT 1

void MutatorFrontendAction_377::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("ConstexprUnsignedMethod")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto rettype = MT->getReturnType();
      auto type_str = rettype.getAsString();
      llvm::outs() << type_str << '\n';
      string new_type_str = type_str;
      if (type_str == "size_t")
        new_type_str = "ssize_t";
      else if (type_str == "uint32_t")
        new_type_str = "int32_t";
      else if (type_str == "uint64_t")
        new_type_str = "int64_t";
      else
        return;
      auto body = MT->getBody();
      auto body_str = stringutils::rangetoStr(*(Result.SourceManager),
                                              body->getSourceRange());
      llvm::outs() << body_str << '\n';
      //Perform mutation on the source code text by applying string replacement
      auto pos = body_str.find("return");
      if (pos == string::npos)
        return;
      auto semicolon_pos = body_str.find(';', pos);
      if (semicolon_pos == string::npos)
        return;
      string new_body_str = body_str.substr(0, pos) + "return -1" +
                            body_str.substr(semicolon_pos);
      llvm::outs() << new_body_str << '\n';
      auto decl_str = stringutils::rangetoStr(*(Result.SourceManager),
                                              MT->getSourceRange());
      string new_decl_str =
          "/*mut377*/" + new_type_str + decl_str.substr(type_str.size());
      new_decl_str.replace(new_decl_str.find(body_str), body_str.size(),
                           new_body_str);
      llvm::outs() << new_decl_str << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), new_decl_str);
    }
}
  
void MutatorFrontendAction_377::MutatorASTConsumer_377::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(isConstexpr(), hasReturnType(isUnsignedInteger())).bind("ConstexprUnsignedMethod");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}