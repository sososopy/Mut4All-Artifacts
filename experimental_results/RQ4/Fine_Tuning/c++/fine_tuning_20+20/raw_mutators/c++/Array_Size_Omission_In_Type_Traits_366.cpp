//header file
#pragma once
#include "Mutator_base.h"

/**
 * Array_Size_Omission_In_Type_Traits_366
 */ 
class MutatorFrontendAction_366 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(366)

private:
    class MutatorASTConsumer_366 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_366(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/array_size_omission_in_type_traits_366.h"

// ========================================================================================================
#define MUT366_OUTPUT 1

void MutatorFrontendAction_366::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TypeTraitExpr>("TypeTrait")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      llvm::outs() << "content: " << content << '\n';
      //Perform mutation on the source code text by applying string replacement
      if (content.find("__array_extent") != string::npos) {
        if (content.find(',') != string::npos)
          content.erase(content.find(','), 1);
        else if (content.find('>') != string::npos)
          content.insert(content.find('>'), ",");
      } else if (content.find("__is_array") != string::npos) {
        if (content.find(',') != string::npos)
          content.erase(content.find(','), 1);
        else if (content.find('>') != string::npos)
          content.insert(content.find('>'), ",");
      } else if (content.find("__is_pointer") != string::npos) {
        if (content.find(',') != string::npos)
          content.erase(content.find(','), 1);
        else if (content.find('>') != string::npos)
          content.insert(content.find('>'), ",");
      } else if (content.find("__is_lvalue_reference") != string::npos) {
        if (content.find(',') != string::npos)
          content.erase(content.find(','), 1);
        else if (content.find('>') != string::npos)
          content.insert(content.find('>'), ",");
      } else if (content.find("__is_rvalue_reference") != string::npos) {
        if (content.find(',') != string::npos)
          content.erase(content.find(','), 1);
        else if (content.find('>') != string::npos)
          content.insert(content.find('>'), ",");
      } else if (content.find("__is_member_object_pointer") != string::npos) {
        if (content.find(',') != string::npos)
          content.erase(content.find(','), 1);
        else if (content.find('>') != string::npos)
          content.insert(content.find('>'), ",");
      } else if (content.find("__is_member_function_pointer") != string::npos) {
        if (content.find(',') != string::npos)
          content.erase(content.find(','), 1);
        else if (content.find('>') != string::npos)
          content.insert(content.find('>'), ",");
      } else if (content.find("__is_enum") != string::npos) {
        if (content.find(',') != string::npos)
          content.erase(content.find(','), 1);
        else if (content.find('>') != string::npos)
          content.insert(content.find('>'), ",");
      } else if (content.find("__is_union") != string::npos) {
        if (content.find(',') != string::npos)
          content.erase(content.find(','), 1);
        else if (content.find('>') != string::npos)
          content.insert(content.find('>'), ",");
      } else if (content.find("__is_class") != string::npos) {
        if (content.find(',') != string::npos)
          content.erase(content.find(','), 1);
        else if (content.find('>') != string::npos)
          content.insert(content.find('>'), ",");
      } else if (content.find("__is_function") != string::npos) {
        if (content.find(',') != string::npos)
          content.erase(content.find(','), 1);
        else if (content.find('>') != string::npos)
          content.insert(content.find('>'), ",");
      } else if (content.find("__is_reference") != string::npos) {
        if (content.find(',') != string::npos)
          content.erase(content.find(','), 1);
        else if (content.find('>') != string::npos)
          content.insert(content.find('>'), ",");
      } else if (content.find("__is_arithmetic") != string::npos) {
        if (content.find(',') != string::npos)
          content.erase(content.find(','), 1);
        else if (content.find('>') != string::npos)
          content.insert(content.find('>'), ",");
      } else if (content.find("__is_fundamental") != string::npos) {
        if (content.find(',') != string::npos)
          content.erase(content.find(','), 1);
        else if (content.find('>') != string::npos)
          content.insert(content.find('>'), ",");
      } else if (content.find("__is_object") != string::npos) {
        if (content.find(',') != string::npos)
          content.erase(content.find(','), 1);
        else if (content.find('>') != string::npos)
          content.insert(content.find('>'), ",");
      } else if (content.find("__is_scalar") != string::npos) {
        if (content.find(',') != string::npos)
          content.erase(content.find(','), 1);
        else if (content.find('>') != string::npos)
          content.insert(content.find('>'), ",");
      } else if (content.find("__is_compound") != string::npos) {
        if (content.find(',') != string::npos)
          content.erase(content.find(','), 1);
        else if (content.find('>') != string::npos)
          content.insert(content.find('>'), ",");
      } else if (content.find("__is_member_pointer") != string::npos) {
        if (content.find(',') != string::npos)
          content.erase(content.find(','), 1);
        else if (content.find('>') != string::npos)
          content.insert(content.find('>'), ",");
      } else if (content.find("__is_const") != string::npos) {
        if (content.find(',') != string::npos)
          content.erase(content.find(','), 1);
        else if (content.find('>') != string::npos)
          content.insert(content.find('>'), ",");
      } else if (content.find("__is_volatile") != string::npos) {
        if (content.find(',') != string::npos)
          content.erase(content.find(','), 1);
        else if (content.find('>') != string::npos)
          content.insert(content.find('>'), ",");
      } else if (content.find("__is_trivial") != string::npos) {
        if (content.find(',') != string::npos)
          content.erase(content.find(','), 1);
        else if (content.find('>') != string::npos)
          content.insert(content.find('>'), ",");
      } else if (content.find("__is_trivially_copyable") != string::npos) {
        if (content.find(',') != string::npos)
          content.erase(content.find(','), 1);
        else if (content.find('>') != string::npos)
          content.insert(content.find('>'), ",");
      } else if (content.find("__is_standard_layout") != string::npos) {
        if (content.find(',') != string::npos)
          content.erase(content.find(','), 1);
        else if (content.find('>') != string::npos)
          content.insert(content.find('>'), ",");
      } else if (content.find("__is_empty") != string::npos) {
        if (content.find(',') != string::npos)
          content.erase(content.find(','), 1);
        else if (content.find('>') != string::npos)
          content.insert(content.find('>'), ",");
      } else if (content.find("__is_polymorphic") != string::npos) {
        if (content.find(',') != string::npos)
          content.erase(content.find(','), 1);
        else if (content.find('>') != string::npos)
          content.insert(content.find('>'), ",");
      } else if (content.find("__is_abstract") != string::npos) {
        if (content.find(',') != string::npos)
          content.erase(content.find(','), 1);
        else if (content.find('>') != string::npos)
          content.insert(content.find('>'), ",");
      } else if (content.find("__is_final") != string::npos) {
        if (content.find(',') != string::npos)
          content.erase(content.find(','), 1);
        else if (content.find('>') != string::npos)
          content.insert(content.find('>'), ",");
      } else if (content.find("__is_aggregate") != string::npos) {
        if (content.find(',') != string::npos)
          content.erase(content.find(','), 1);
        else if (content.find('>') != string::npos)
          content.insert(content.find('>'), ",");
      } else if (content.find("__is_signed") != string::npos) {
        if (content.find(',') != string::npos)
          content.erase(content.find(','), 1);
        else if (content.find('>') != string::npos)
          content.insert(content.find('>'), ",");
      } else if (content.find("__is_unsigned") != string::npos) {
        if (content.find(',') != string::npos)
          content.erase(content.find(','), 1);
        else if (content.find('>') != string::npos)
          content.insert(content.find('>'), ",");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_366::MutatorASTConsumer_366::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typeTraitExpr().bind("TypeTrait");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}