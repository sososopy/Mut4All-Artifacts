//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutator_Constexpr_Address_Reference_419
 */ 
class MutatorFrontendAction_419 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(419)

private:
    class MutatorASTConsumer_419 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_419(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl*> constexpr_vars;
        std::vector<const clang::CXXRecordDecl*> constexpr_structs;
    };
};

//source file
#include "../include/Mutator_Constexpr_Address_Reference_419.h"

// ========================================================================================================
#define MUT419_OUTPUT 1

void MutatorFrontendAction_419::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::VarDecl>("ConstexprVar")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Check if it's a constexpr variable declaration
      if (!MT->isConstexpr())
        return;
      //Get the initializer expression
      const Expr* init = MT->getInit();
      if (!init)
        return;
      //Check if initializer is not a simple literal (to avoid trivial cases)
      if (isa<IntegerLiteral>(init) || isa<FloatingLiteral>(init) || isa<StringLiteral>(init) || isa<CharacterLiteral>(init))
        return;
      //Record candidate constexpr variables for later use
      constexpr_vars.push_back(MT);
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ConstexprStruct")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Check if it's a struct/class with constexpr instances
      if (!MT->isCompleteDefinition())
        return;
      //Record candidate structs for later use
      constexpr_structs.push_back(MT);
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::VarDecl>("TargetVar")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Check if it's a constexpr variable declaration with non-trivial initializer
      if (!MT->isConstexpr())
        return;
      const Expr* init = MT->getInit();
      if (!init)
        return;
      //Skip if initializer is a simple literal
      if (isa<IntegerLiteral>(init) || isa<FloatingLiteral>(init) || isa<StringLiteral>(init) || isa<CharacterLiteral>(init))
        return;
      //Get the source code text of target node
      std::string original_text = stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
      //Find position of initializer in the source text
      std::string init_text = stringutils::rangetoStr(*(Result.SourceManager), init->getSourceRange());
      size_t init_pos = original_text.find(init_text);
      if (init_pos == std::string::npos)
        return;
      //Prepare mutation options based on available constexpr variables/structs
      std::vector<std::string> mutation_options;
      //Option 1: Use pointer arithmetic with array if available
      for (const VarDecl* var : constexpr_vars) {
        QualType var_type = var->getType();
        if (var_type->isArrayType()) {
          std::string var_name = var->getNameAsString();
          //Create expression like *(arr + x - offset)
          //We need to find a suitable offset - use another constexpr variable or literal
          std::string offset = "0";
          for (const VarDecl* other_var : constexpr_vars) {
            if (other_var != var && other_var->getType()->isIntegerType()) {
              offset = other_var->getNameAsString();
              break;
            }
          }
          mutation_options.push_back("*(" + var_name + " + " + offset + ")");
        }
      }
      //Option 2: Use address of struct member if available
      for (const CXXRecordDecl* struct_decl : constexpr_structs) {
        //Find constexpr instances of this struct
        for (const VarDecl* var : constexpr_vars) {
          QualType var_type = var->getType();
          if (var_type->getAsCXXRecordDecl() == struct_decl) {
            std::string var_name = var->getNameAsString();
            //Find a public member field
            for (FieldDecl* field : struct_decl->fields()) {
              if (field->getAccess() == AS_public || field->getAccess() == AS_none) {
                std::string field_name = field->getNameAsString();
                mutation_options.push_back("(&" + var_name + ")->" + field_name);
                break;
              }
            }
            break;
          }
        }
      }
      //Option 3: Use address of another constexpr variable
      if (constexpr_vars.size() > 1) {
        for (const VarDecl* var : constexpr_vars) {
          if (var != MT && var->getType()->isIntegerType()) {
            std::string var_name = var->getNameAsString();
            mutation_options.push_back("*&" + var_name);
            break;
          }
        }
      }
      //If no mutation options available, skip
      if (mutation_options.empty())
        return;
      //Randomly select a mutation option
      int choice = getrandom::getRandomIndex(mutation_options.size() - 1);
      std::string new_init = mutation_options[choice];
      //Perform mutation on the source code text by applying string replacement
      std::string mutated_text = original_text;
      mutated_text.replace(init_pos, init_text.length(), new_init);
      mutated_text = "/*mut419*/" + mutated_text;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutated_text);
    }
}
  
void MutatorFrontendAction_419::MutatorASTConsumer_419::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    //First matcher to collect all constexpr variables
    DeclarationMatcher constexpr_var_matcher = varDecl(isConstexpr()).bind("ConstexprVar");
    //Second matcher to collect struct/class definitions that might have constexpr instances
    DeclarationMatcher struct_matcher = cxxRecordDecl().bind("ConstexprStruct");
    //Third matcher to identify target constexpr variables for mutation
    DeclarationMatcher target_matcher = varDecl(isConstexpr(), unless(hasInit(anyOf(
        integerLiteral(),
        floatingLiteral(),
        stringLiteral(),
        characterLiteral()
    )))).bind("TargetVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(constexpr_var_matcher, &callback);
    matchFinder.addMatcher(struct_matcher, &callback);
    matchFinder.addMatcher(target_matcher, &callback);
    matchFinder.matchAST(Context);
}