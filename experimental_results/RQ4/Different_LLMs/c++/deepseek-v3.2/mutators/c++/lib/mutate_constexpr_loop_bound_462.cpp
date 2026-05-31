//source file
#include "../include/Mutate_Constexpr_Loop_Bound_462.h"

// ========================================================================================================
#define MUT462_OUTPUT 1

void MutatorFrontendAction_462::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FS = Result.Nodes.getNodeAs<clang::ForStmt>("ForStmt")) {
      //Filter nodes in header files
      if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FS->getBeginLoc()))
        return;
      
      // Get the loop's condition expression
      auto *Cond = FS->getCond();
      if (!Cond) return;
      
      // Check if the loop is inside a constexpr function
      auto *ParentFunc = Result.Context->getParents(*FS).begin()->get<clang::FunctionDecl>();
      if (!ParentFunc || !ParentFunc->isConstexpr()) return;
      
      // Check if the loop body contains container mutating operations
      auto *Body = FS->getBody();
      if (!Body) return;
      
      // Simple check: look for calls to push_back, insert, emplace_back in the body
      bool hasContainerMutation = false;
      for (auto Child : Body->children()) {
        if (auto *CE = dyn_cast<clang::CallExpr>(Child)) {
          auto *Callee = CE->getDirectCallee();
          if (Callee) {
            auto Name = Callee->getName();
            if (Name == "push_back" || Name == "insert" || Name == "emplace_back") {
              hasContainerMutation = true;
              break;
            }
          }
        }
      }
      if (!hasContainerMutation) return;
      
      // Get the original upper bound expression
      auto CondRange = Cond->getSourceRange();
      auto CondText = stringutils::rangetoStr(*(Result.SourceManager), CondRange);
      
      // Extract the comparison expression (assuming format like "i < N")
      // We'll replace N with mutated values
      size_t cmpPos = CondText.find('<');
      if (cmpPos == std::string::npos) return;
      
      // Get the part after '<' (the upper bound)
      std::string UpperBoundText = CondText.substr(cmpPos + 1);
      // Trim whitespace
      UpperBoundText.erase(0, UpperBoundText.find_first_not_of(" "));
      UpperBoundText.erase(UpperBoundText.find_last_not_of(" ") + 1);
      
      // Generate mutated bounds
      std::vector<std::string> mutatedBounds;
      mutatedBounds.push_back(UpperBoundText + "-1");   // N-1
      mutatedBounds.push_back(UpperBoundText);         // N (original)
      mutatedBounds.push_back(UpperBoundText + "+1");   // N+1
      mutatedBounds.push_back("2*" + UpperBoundText);  // 2*N
      mutatedBounds.push_back("57");                   // 57 (as in bug report)
      
      // Randomly choose one mutation
      int choice = getrandom::getRandomIndex(mutatedBounds.size() - 1);
      std::string newBound = mutatedBounds[choice];
      
      // Construct new condition text
      std::string newCondText = CondText.substr(0, cmpPos + 1) + " " + newBound;
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CondRange), newCondText);
    }
}
  
void MutatorFrontendAction_462::MutatorASTConsumer_462::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher mat2cher = forStmt().bind("ForStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(mat2cher, &callback);
    matchFinder.matchAST(Context);
}