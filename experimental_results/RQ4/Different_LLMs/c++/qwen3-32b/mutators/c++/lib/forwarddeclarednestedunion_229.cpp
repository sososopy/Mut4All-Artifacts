//source file
#include "../include/ForwardDeclaredNestedUnion_229.h"

// ========================================================================================================
#define MUT229_OUTPUT 1

void MutatorFrontendAction_229::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *UD = Result.Nodes.getNodeAs<clang::RecordDecl>("unionDecl")) {
        //Filter nodes in header files
        if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       UD->getLocation()))
            return;
        //Get the source code text of target node
        std::string unionName = UD->getNameAsString();
        if (unionName.empty())
            return;

        // Generate a new name for the typedef union
        std::string newUnionName = "Mut_229";

        SourceLocation loc = UD->getBeginLoc();

        // Insert forward declaration
        std::string forwardDecl = "union " + unionName + ";";
        Rewrite.InsertTextBefore(loc, forwardDecl + "\n");

        // Insert the new typedef union
        std::string newUnion = "typedef union __attribute__((__transparent_union__)) {\n    union " + unionName + " " + unionName + ";\n} " + newUnionName + ";\n";
        Rewrite.InsertTextBefore(loc, newUnion);
    }
}
  
void MutatorFrontendAction_229::MutatorASTConsumer_229::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = recordDecl(isUnion()).bind("unionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}