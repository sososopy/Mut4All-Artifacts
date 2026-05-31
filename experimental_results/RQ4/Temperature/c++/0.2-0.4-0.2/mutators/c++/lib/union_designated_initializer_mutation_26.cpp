//source file
#include "../include/union_designated_initializer_mutation_26.h"

// ========================================================================================================
#define MUT26_OUTPUT 1

void MutatorFrontendAction_26::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("UnionVar")) {
        // Filter nodes in header files
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       VD->getLocation()))
            return;

        // Ensure the variable is of union type
        if (const clang::RecordType *RT = VD->getType()->getAs<clang::RecordType>()) {
            if (RT->getDecl()->isUnion()) {
                const clang::InitListExpr *ILE = dyn_cast<clang::InitListExpr>(VD->getInit());
                if (ILE && ILE->getNumInits() == 1) {
                    // Get the source code text of target node
                    std::string initText = stringutils::rangetoStr(*(Result.SourceManager),
                                                                   ILE->getSourceRange());
                    // Perform mutation on the source code text by applying string replacement
                    initText.insert(initText.find('}') - 1, ", .b = 20.5");
                    // Replace the original AST node with the mutated one
                    Rewrite.ReplaceText(CharSourceRange::getTokenRange(ILE->getSourceRange()), initText);
                }
            }
        }
    }
}

void MutatorFrontendAction_26::MutatorASTConsumer_26::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasInitializer(initListExpr()),
                                         hasType(recordType(hasDeclaration(recordDecl(isUnion())))))
                                 .bind("UnionVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}