//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Nested_Template_Parameter_In_Concept_182
 */ 
class MutatorFrontendAction_182 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(182)

private:
    class MutatorASTConsumer_182 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_182(Rewriter &R) : TheRewriter(R) {}
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
        bool void_t_declared = false;
        SourceLocation global_insert_loc;
    };
};

//source file
#include "../include/Mutator_Insert_Nested_Template_Parameter_In_Concept_182.h"

// ========================================================================================================
#define MUT182_OUTPUT 1

void MutatorFrontendAction_182::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CR = Result.Nodes.getNodeAs<clang::ConceptDecl>("ConceptDecl")) {
      //Filter nodes in header files
      if (!CR || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CR->getLocation()))
        return;
      
      // Ensure void_t is declared globally
      if (!void_t_declared) {
        // Find a suitable location to insert void_t declaration (before the concept)
        SourceManager &SM = Rewrite.getSourceMgr();
        global_insert_loc = SM.getLocForStartOfFile(SM.getMainFileID());
        // Insert void_t template at global scope
        std::string void2_decl = "\ntemplate <class...> struct void_t { using type = void; };\n";
        Rewrite.InsertText(global_insert_loc, void2_decl);
        void_t_declared = true;
      }

      // Get the requires expression body
      const auto *CE = CR->getConstraintExpr();
      if (!CE) return;
      
      // Traverse the constraint expression to find type requirements
      class TypeRequirementVisitor : public RecursiveASTVisitor<TypeRequirementVisitor> {
      public:
        std::vector<const TypeRequirement *> typeReqs;
        bool VisitTypeRequirement(const TypeRequirement *TR) {
          typeReqs.push_back(TR);
          return true;
        }
      } visitor;
      visitor.TraverseStmt(const_cast<Expr*>(CE));
      
      if (visitor.typeReqs.empty()) return;
      
      // Randomly select one type requirement to mutate
      size_t idx = getrandom::getRandomIndex(visitor.typeReqs.size() - 1);
      const TypeRequirement *selectedReq = visitor.typeReqs[idx];
      
      // Get the source range of the type requirement
      SourceRange reqRange = selectedReq->getSourceRange();
      if (!reqRange.isValid()) return;
      
      // Get the original text
      std::string origText = stringutils::rangetoStr(*(Result.SourceManager), reqRange);
      
      // Extract the nested name (e.g., "T::inner_type")
      // We need to parse the typename requirement text
      // The format is "typename <nested-name>"
      size_t typename_pos = origText.find("typename");
      if (typename_pos == std::string::npos) return;
      
      std::string nested_name = origText.substr(typename_pos + 8); // "typename " is 8 chars
      // Trim whitespace
      nested_name.erase(0, nested_name.find_first_not_of(" \t"));
      nested_name.erase(nested_name.find_last_not_of(" \t") + 1);
      
      // Remove trailing semicolon if present
      if (!nested_name.empty() && nested_name.back() == ';')
        nested_name.pop_back();
      
      // Insert template keyword before the identifier
      size_t dcolon = nested_name.find("::");
      if (dcolon == std::string::npos) return;
      
      std::string mutated_name = nested_name.substr(0, dcolon + 2) + "template " + nested_name.substr(dcolon + 2);
      
      // Build the mutated requirement: typename void_t<...>
      std::string mutated_req = "typename void_t<" + mutated_name + ">";
      
      // Replace the original text
      Rewrite.ReplaceText(reqRange, mutated_req);
    }
}
  
void MutatorFrontendAction_182::MutatorASTConsumer_182::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = conceptDecl().bind("ConceptDecl");
    Callback callback(TheRewriter);
    matchFinder.addMat2cher(matcher, &callback);
    matchFinder.matchAST(Context);
}